#include "TimeUtils.h"
#include <chrono>
#include <iostream>

std::atomic<uint64_t> Time::OffSetTime(0);

uint64_t Time::GetNow()
{
	return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count() + OffSetTime.load();
}

uint64_t Time::GetNowMS()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() + OffSetTime.load()* 1000;
}

void Time::FormatTime(tm * pTm, uint64_t iTimeSecond)
{
	localtime_s(pTm, (int64_t const*)&iTimeSecond);
}

void Time::FormatTimeMS(tm * pTm, uint64_t iTimeSecond)
{
	FormatTime(pTm, iTimeSecond / 1000);
}

uint64_t Time::MakeTime(tm * pTm)
{
	return (uint64_t)mktime(pTm);
}

// 
// Timer::Timer()
// 	:m_pThread(nullptr)
// {
// 	m_bRun.store(true);
// 	m_Handle = [](std::unordered_set< uint32_t >&)
// 	{
// 
// 	};
// }
// 
// Timer::~Timer()
// {
// 	if (m_pThread != nullptr)
// 	{
// 		Stop();
// 		delete m_pThread;
// 	}
// }
// 
// void Timer::AddTimer(uint64_t iTimeMS, uint32_t iActorId)
// {
// 	std::unique_lock<std::mutex> lock(m_Mutex);
// 	m_NewTmer[iTimeMS].insert(iActorId);
// }
// 
// void Timer::SetTimerHandle(std::function<void(std::unordered_set< uint32_t >&)>& handle)
// {
// 	m_Handle = handle;
// }
// 
// void Timer::Run()
// {
// 	m_pThread = new std::thread([this]()
// 	{
// 		while (m_bRun)
// 		{
// 			RunOnce();
// 		}
// 	});
// }
// 
// void Timer::Stop()
// {
// 	if (m_pThread != nullptr)
// 	{
// 		m_bRun.store(false);
// 		m_pThread->join();
// 		delete m_pThread;
// 		m_pThread = nullptr;
// 	}
// }
// 
// void Timer::RunOnce()
// {
// 	MapTimer mapTemp;
// 	{
// 		std::unique_lock<std::mutex> lock(m_Mutex);
// 		std::swap(mapTemp, m_NewTmer);
// 	}
// 	for (auto it : mapTemp)
// 	{
// 		m_mapTimer[it.first].insert(it.second.begin(), it.second.end());
// 	}
// 	uint64_t iNowMs = Time::GetNowMS();
// 	while (!m_mapTimer.empty())
// 	{
// 		auto it = m_mapTimer.begin();
// 		if (iNowMs < it->first)
// 			break;
// 
// 		std::unordered_set< uint32_t > vID;
// 		std::swap(vID, it->second);
// 		m_mapTimer.erase(it);
// 		m_Handle(vID);
// 	}
// }
//
//====================================================================

class SubTimerEntity
{
public:
	SubTimerEntity(ActorTimer * pActorTimer)
		: m_pActorTimer(pActorTimer)
		, m_bDelete(false)
		, m_bHandleing(false)
	{
		m_pActorTimer->RegistSubTimer(this);
	}
	~SubTimerEntity()
	{
		m_pActorTimer->UnRegistSubTimer(this);
	}

	void	AddTimer(uint64_t iTimeMS, const SubTimer::TimerHandle& handel)
	{
		uint64_t iNextTime = iTimeMS + Time::GetNowMS();
		if (m_mTimer.empty() || m_mTimer.begin()->first > iNextTime)
		{
			m_pActorTimer->AddTimer(iNextTime, this);
		}
		m_mTimer[iNextTime].push_back(handel);
	}

	void	OnTimer(uint64_t iNowMS)
	{
		m_bHandleing = true;
		while (!m_mTimer.empty() && !m_bDelete)
		{
			auto itBegin = m_mTimer.begin();
			if (itBegin->first > iNowMS)
				return;
			std::list<SubTimer::TimerHandle> listHandle;
			std::swap(listHandle, itBegin->second);
			m_mTimer.erase(itBegin);
			while (!listHandle.empty() && !m_bDelete)
			{
				SubTimer::TimerHandle& stHanle = listHandle.front();
				stHanle(iNowMS);
				listHandle.pop_front();
			}
		}
		m_bHandleing = false;
		if (m_bDelete)
		{
			delete this;
		}
	}

	void	Clear()
	{
		m_mTimer.clear();
		m_pActorTimer->UnRegistSubTimer(this);
		m_pActorTimer->RegistSubTimer(this);
	}

	void	Delete()
	{
		m_bDelete = true;
		if (!m_bHandleing)
		{
			delete this;
		}
	}


private:
	std::map<uint64_t, std::list<SubTimer::TimerHandle>> m_mTimer;
	bool	m_bDelete;
	bool	m_bHandleing;
public:
	ActorTimer* m_pActorTimer;
};

//====================================================================

ActorTimer::ActorTimer()
{
	OnAddTimer = [](uint64_t) {};
}

ActorTimer::~ActorTimer()
{
}

void ActorTimer::RegistSubTimer(SubTimerEntity * pSubTimer)
{
	m_setSubTimer.insert(pSubTimer);
}

void ActorTimer::UnRegistSubTimer(SubTimerEntity * pSubTimer)
{
	m_setSubTimer.erase(pSubTimer);
	for (auto it=m_mTimer.begin(); it!=m_mTimer.end();)
	{
		it->second.erase(pSubTimer);
		if (it->second.empty())
		{
			m_mTimer.erase(it++);
		} 
		else
		{
			++it;
		}
	}

}

void ActorTimer::OnTimer(uint64_t iNowMS)
{
	while (!m_mTimer.empty())
	{
		auto itBegin = m_mTimer.begin();
		if (itBegin->first > iNowMS)
			return;
		std::set<SubTimerEntity*> setTimer;
		std::swap(setTimer, itBegin->second);
		m_mTimer.erase(itBegin);
		for (auto& it:setTimer)
		{
			if (m_setSubTimer.find(it) != m_setSubTimer.end())
			{
				it->OnTimer(iNowMS);
			}
		}
	}
}

void ActorTimer::AddTimer(uint64_t iTimeMS, SubTimerEntity * pSubTimer)
{
	if (m_mTimer.empty() || m_mTimer.begin()->first > iTimeMS)
	{
		OnAddTimer(iTimeMS);
	}
	m_mTimer[iTimeMS].insert(pSubTimer);
}

//==============================================================
SubTimer::SubTimer(ActorTimer * pActorTimer)
{
	m_pSubEntity = new SubTimerEntity(pActorTimer);
}

SubTimer::~SubTimer()
{
	if (m_pSubEntity != nullptr)
		m_pSubEntity->Delete();
}

SubTimer::SubTimer(const SubTimer & other)
{
	m_pSubEntity = new SubTimerEntity(other.m_pSubEntity->m_pActorTimer);
}

SubTimer::SubTimer(SubTimer && other)
{
	m_pSubEntity = other.m_pSubEntity;
	other.m_pSubEntity = nullptr;
}

void SubTimer::AddTimer(uint64_t iTimeMS, const TimerHandle & handel)
{
	m_pSubEntity->AddTimer(iTimeMS, handel);
}

void SubTimer::OnTimer(uint64_t iNowMS)
{
	m_pSubEntity->OnTimer(iNowMS);
}

void SubTimer::Clear()
{
	m_pSubEntity->Clear();
}
