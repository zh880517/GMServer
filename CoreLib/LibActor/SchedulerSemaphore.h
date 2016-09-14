#pragma once
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <vector>
#include <list>
#include <LibActor/IScheduler.h>

namespace ActorLite
{
	/*
		通过C++11的mutex和condition_variable模拟的信号量进行消息调度
	*/
	class SchedulerSemaphore :public IScheduler
	{
	public:
		SchedulerSemaphore(uint32_t nMaxThread = 0);
		~SchedulerSemaphore();

		virtual bool	Init() override;

		virtual void	Schedule(Mailbox* pMailBox) override;

		virtual void	Destroy() override;

	protected:

		Mailbox*		Pop();
		
		static	void	ThreadFunc(void *const context);

	private:
		std::atomic_bool					m_bRuning;
		std::list<Mailbox*>					m_listWaite;
		std::condition_variable				m_Condition;
		std::mutex							m_Mutex;
		std::vector<std::thread>			m_vThread;
		uint32_t							m_iMaxThreadNum;
	};


}
