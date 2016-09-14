#pragma once
#include <LibActor/Mailbox.h>
#include <LibActor/Directory.h>
#include <LibActor/SharePtrDef.h>
namespace ActorLite
{
	class IScheduler;
	class Framework
	{
	public:
		Framework(IScheduler* pScheduler);
		~Framework();

		bool				Init();

		void				RegisterActor(Actor* pActor, const Detail::Name& name);
		void				UnRegisterActor(Actor *const pActor);
		void				RunOnce();
		bool				SendMsg(IMessagePtr& pMsg, const Detail::Address& adress);
		bool				SendMsg(IMessagePtr& pMsg, std::list<Detail::Address>& listAdress);
		void				Destroy();
		bool				SystemMsg(IMessagePtr& pMsg);
		bool				IsRuning() { return !m_bDestory; }
	protected:
		bool				SendEndPoint(IMessagePtr& pMsg, const Detail::Address& adress);
		bool				SendEndPoint(IMessagePtr& pMsg, std::list<Detail::Address>& listAdress);
		bool				SendInternal(IMessagePtr& pMsg, const Detail::Address& adress);
	private:

		bool						m_bDestory;

		Detail::Directory<Mailbox>	m_MailBoxes;

		IScheduler*					m_pScheduler;

		Detail::Address				m_Address;
		Detail::Name				m_Name;

		EndPointPtr					m_pEndPoint;
		IOServicePoolPtr			m_pIoServicePool;

		//ϵͳ��Ϣ
		using MsgList = std::list< IMessagePtr >;
		std::mutex					m_ReciveMutex;	//���ն�����
		MsgList						m_listRecive;//������Ϣ�Ķ��У���Ҫ����
		MsgList						m_listHandle;

		//ϵͳ��Ϣ�Ĵ����࣬��ʹ���������
		friend class SystemMsgHandle;
	};

}

