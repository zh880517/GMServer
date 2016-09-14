#include <LibActor/SchedulerSemaphore.h>
#include <LibActor/Mailbox.h>

namespace ActorLite
{
	SchedulerSemaphore::SchedulerSemaphore(uint32_t nMaxThread)
	{
		if (nMaxThread  == 0)
		{
			nMaxThread = std::thread::hardware_concurrency() * 2;
			if (nMaxThread == 0)
			{
				nMaxThread = 4;
			}
			m_iMaxThreadNum = nMaxThread;
		}
	}


	SchedulerSemaphore::~SchedulerSemaphore()
	{
	}

	bool SchedulerSemaphore::Init()
	{
		m_bRuning.store(1);
		for (uint32_t i = 0; i < m_iMaxThreadNum; ++i)
		{
			m_vThread.push_back(std::thread(ThreadFunc, (void*)this));
		}
		return true;
	}

	void SchedulerSemaphore::Schedule(Mailbox* pMailBox)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_listWaite.push_back(pMailBox);
		m_Condition.notify_one();
	}

	void SchedulerSemaphore::Destroy()
	{
		m_bRuning.store(false);
		m_Condition.notify_all();
		for (uint32_t i = 0; i < m_vThread.size(); ++i)
		{
			m_vThread[i].join();
		}
	}

	Mailbox* SchedulerSemaphore::Pop()
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		if (m_listWaite.empty())
		{
			m_Condition.wait(lock);
		}
		std::list<Mailbox*> listBusy;
		Mailbox* pBox = nullptr;
		while (!m_listWaite.empty())
		{
			pBox = m_listWaite.front();
			m_listWaite.pop_front();
			if (pBox != nullptr)
			{
				if (!pBox->IsHandling())
				{
					break;
				}
				else
				{
					listBusy.push_back(pBox);
				}
			}
			pBox = nullptr;
		}
		if (listBusy.size() > 0)
		{
			m_listWaite.splice(m_listWaite.end(), listBusy);
		}
		return pBox;
	}

	void SchedulerSemaphore::ThreadFunc(void *const context)
	{
		SchedulerSemaphore* pSheduler = (SchedulerSemaphore*)context;
		while (pSheduler->m_bRuning == 1)
		{
			Mailbox* pBox = pSheduler->Pop();
			if (pBox != nullptr)
			{
				pBox->Process();
			}
		}
	}

}