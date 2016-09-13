#pragma once
#include <functional>
#include <stdint.h>
#include <map>
#include <set>
#include <unordered_set>
#include <atomic>
#include <mutex>
#include <thread>

class Time
{
public:
	static	uint64_t	GetNow();
	static	uint64_t	GetNowMS();

	static	void	FormatTime(tm* pTm, uint64_t iTimeSecond);

	static	void	FormatTimeMS(tm* pTm, uint64_t iTimeSecond);

	static	uint64_t	MakeTime(tm* pTm);

	static	std::atomic<uint64_t>	OffSetTime;
};

class SubTimer;
class SubTimerEntity;
class ActorTimer
{
public:
	ActorTimer();
	~ActorTimer();

	void	RegistSubTimer(SubTimerEntity* pSubTimer);

	void	UnRegistSubTimer(SubTimerEntity* pSubTimer);

	void	OnTimer(uint64_t iNowMS);

	void	AddTimer(uint64_t iTimeMS, SubTimerEntity* pSubTimer);

public:

	std::function<void(uint64_t)> OnAddTimer;

private:
	std::set<SubTimerEntity*> m_setSubTimer;
	std::map<uint64_t, std::set<SubTimerEntity*>> m_mTimer;
};


class SubTimer
{
public:
	using TimerHandle = std::function<void(uint64_t)>;
public:
	SubTimer(ActorTimer* pActorTimer);
	~SubTimer();

	SubTimer(const SubTimer& other);
	SubTimer(SubTimer&& other);


	void	AddTimer(uint64_t iTimeMS, const TimerHandle& handel);

	void	OnTimer(uint64_t iNowMS);

	void	Clear();

private:
	SubTimerEntity*	m_pSubEntity;
};


// class Timer
// {
// public:
// 	Timer();
// 	~Timer();
// 
// 	//在Run调用之前设置回调函数
// 	void	SetTimerHandle(std::function<void(std::unordered_set< uint32_t >&)>& handle);
// 
// 	void	AddTimer(uint64_t iTimeMS, uint32_t iActorId);
// 
// 	void	Run();
// 
// 	void	Stop();
// 
// protected:
// 
// 	void	RunOnce();
// 
// private:
// 	using MapTimer = std::map<uint64_t, std::unordered_set< uint32_t > >;
// 	std::atomic_bool				m_bRun;
// 	std::mutex						m_Mutex;
// 	MapTimer						m_NewTmer;
// 	MapTimer						m_mapTimer;
// 	std::function<void(std::unordered_set< uint32_t >&)>	m_Handle;
// 	std::thread*					m_pThread;
// };