#pragma once

namespace ActorLite
{
	class Mailbox;

// 	class IScheduler
// 	{
// 	public:
// 		typedef std::list<Mailbox*> MailBoxList;
// 		IScheduler(){}
// 		virtual	~IScheduler(){}
// 
// 		virtual	void		Init() = 0;
// 
// 		inline	void		Schedule(Mailbox* pMailBox);
// 		//ÿ��ѭ������
// 		inline	void		RunOnce();
// 		/*
// 			Dispatch������ʵ�֣�����ϵͳʹ����ɶ˿ڻ���epollʵ���̵߳���
// 		*/
// 		virtual void		Dispatch(MailBoxList& mailBoxlist) = 0;
// 
// 		virtual	void		Destroy() = 0;
// 
// 	private:
// 		MailBoxList		m_WaitList;
// 		Mutex			m_Mutex;
// 	};
// 
// 	void IScheduler::Schedule(Mailbox* pMailBox)
// 	{
// 		m_Mutex.Lock();
// 		m_WaitList.push_back(pMailBox);
// 		m_Mutex.Unlock();
// 	}

	class IScheduler
	{
	public:
		IScheduler(){}
		virtual	~IScheduler(){}
		virtual	bool		Init() = 0;
		virtual	void		Schedule(Mailbox* pMailBox) = 0;
		virtual	void		RunOnce(){}
		virtual	void		Destroy() = 0;
	};

}