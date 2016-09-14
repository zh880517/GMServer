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

		//���Խӿڣ���ʵ������
		void						DebugOut();
	private:
		using MsgList =	std::list< IMessagePtr >;

		IScheduler*				m_pScheduler;

		std::mutex				m_ReciveMutex;	//���ն�����
		MsgList					m_listRecive;//������Ϣ�Ķ��У���Ҫ����


		std::mutex				m_ActorMutex;//ACTOR������������Actor��ʱ��Ҫ�ȵ���Ϣ������ſ���ɾ��
		Actor*					m_pActor;
		MsgList					m_listHandle;//������Ϣ�Ķ��У������������Ϊֻ��һ���߳��ڴ���
		std::atomic<uint32_t>	m_nUnHanle;//��Ϣ������е�ʣ������

		Detail::Name			m_Name;
		Detail::Address			m_Address;

	};
	/*
		�õ�����������������Ϊ��ÿ�δ�����Ϣ��ʱ��������Ĵ��������ҷ�ֹ��Ϣ����ռ���̵߳�
	*/
	
}
