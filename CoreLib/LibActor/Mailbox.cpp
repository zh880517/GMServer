#include <LibActor/Mailbox.h>
#include <LibActor/Actor.h>
#include <LibActor/IScheduler.h>
#include <iostream>
namespace ActorLite
{
	Mailbox::Mailbox()
		: m_pActor(nullptr)
		, m_pScheduler(nullptr)
	{
		m_nUnHanle.store(0);
	}


	Mailbox::~Mailbox()
	{
		m_listRecive.clear();
		m_listHandle.clear();
	}

	bool Mailbox::RegisterActor(Actor* pActor)
	{
		//Actor��ע���ɾ����Ҫ�����߳����洦���������ﲻ�ü���
		ASSERT_UTILS(pActor != nullptr);
		m_pActor = pActor;
		m_pActor->m_Address = Detail::Address(m_Address);
		return true;
	}

	void Mailbox::UnRegisterActor()
	{
		//Ϊ�˷�ֹ����Ϣ�����ʱ����õ�������Ҫ����
		std::unique_lock<std::mutex> autoLock(m_ActorMutex);
		{
			std::unique_lock<std::mutex> autoMsgLock(m_ReciveMutex);
			m_pActor = nullptr;
			//���Actor��ʱ��Ҫ���δ�������Ϣ
			m_listRecive.clear();
		}
	}

	bool Mailbox::PushMsg(IMessagePtr& pMsg)
	{
		ASSERT_UTILS(m_pScheduler != 0);
		std::unique_lock<std::mutex> autoLock(m_ReciveMutex);
		if (m_pActor == nullptr)
		{
			return false;
		}
		m_listRecive.push_back(pMsg);
		if (m_listRecive.size() == 1)
		{
			m_pScheduler->Schedule(this);
		}
		return true;
	}

	void Mailbox::Process()
	{
		std::unique_lock<std::mutex> autoLock(m_ActorMutex);
		try
		{
			m_ReciveMutex.lock();
			m_listHandle.swap(m_listRecive);
			m_ReciveMutex.unlock();


			m_pActor->OnBeginMsgHandle();
			m_nUnHanle = (uint32_t)m_listHandle.size();
			while (m_listHandle.size() > 0)
			{
				IMessagePtr& pMsg = m_listHandle.front();
				if (nullptr != pMsg)
				{
					if (!m_pActor->m_HandleCollection.Handle(pMsg))
					{
						//����ʧ��˵��û��ע�ᴦ����
					}
				}
				m_listHandle.pop_front();
				--m_nUnHanle;
			}
			m_pActor->OnEndMsgHandle();
		}
		catch (std::exception &/*e*/)
		{

		}
	}

	bool Mailbox::IsUsed()
	{
		return m_pActor == nullptr;
	}

	void Mailbox::DebugOut()
	{
		if (m_listRecive.size() > 0 || m_listHandle.size() > 0)
		{
			std::cout << " MailBox : " << m_Address.m_Brief.m_Index << " m_listRecive size = " << m_listRecive.size() << "m_listHandle size = " << m_listHandle.size();
		}
	}

}
