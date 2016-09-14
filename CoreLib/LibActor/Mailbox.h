#pragma once

#include <list>
#include <mutex>
#include <atomic>
#include <LibActor/Name.h>
#include <LibActor/Message/IMessage.h>

namespace ActorLite
{
	class Actor;
	class IScheduler;

	class Mailbox
	{
	public:
		Mailbox();
		~Mailbox();

	public:
		const	Detail::Name&		GetName(){ return m_Name; }
		void						SetName(const Detail::Name& name){ m_Name = name; }

		const	Detail::Address&	GetAddress(){ return m_Address; }
		void						SetAddress(const Detail::Address& index){ m_Address = index; }

		void						SetScheduler(IScheduler* pScheduler) { m_pScheduler = pScheduler; }

		Actor*						GetActor() { return m_pActor; }
		bool						RegisterActor(Actor* pActor);
		void						UnRegisterActor();

		bool						IsHandling() { return m_nUnHanle.load() != 0; }
		bool						PushMsg(IMessagePtr& pMsg);
		void						Process();
		bool						IsUsed();

		//调试接口，无实际意义
		void						DebugOut();
	private:
		using MsgList =	std::list< IMessagePtr >;

		IScheduler*				m_pScheduler;

		std::mutex				m_ReciveMutex;	//接收队列锁
		MsgList					m_listRecive;//接收消息的队列，需要加锁


		std::mutex				m_ActorMutex;//ACTOR操作锁，销毁Actor的时候要等到消息处理完才可以删除
		Actor*					m_pActor;
		MsgList					m_listHandle;//处理消息的队列，无需加锁，因为只在一个线程内处理
		std::atomic<uint32_t>	m_nUnHanle;//消息处理队列的剩余数量

		Detail::Name			m_Name;
		Detail::Address			m_Address;

	};
	/*
		用到两个队列两个锁是为了每次处理消息的时候减少锁的次数，并且防止消息过多占用线程的
	*/
	
}
