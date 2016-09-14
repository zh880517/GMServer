#pragma once
#include <SharePtrDef.h>
#include <Message/HandlerCollection.h>
#include <Detail/Mutex.h>
namespace ActorLite
{
	class Framework;
	//系统消息处理是在主线程进行的，所以是线程安全的
	class ISystemMsgHandle
	{
	public:
		ISystemMsgHandle(Framework& rfFramework): m_rfFramework(rfFramework){}
		virtual ~ISystemMsgHandle() {}

		void	HandleMsg(IMessagePtr& pMsg);

		virtual void	OnBeginMsgHandle() {}

		virtual void	OnEndMsgHandle() {}

	protected:
		template <class ActorType, class ValueType>
		inline				bool	RegisterHandler(ActorType *const pActor, void (ActorType::*handler)(const ValueType &message, const Detail::Address& from), const char* pInfor = nullptr);

		Framework&				m_rfFramework;
		HandlerCollection		m_HandleCollection;//消息处理
	private:
		ISystemMsgHandle(const ISystemMsgHandle &other);
		ISystemMsgHandle &operator=(const ISystemMsgHandle &other);
	};

	void ISystemMsgHandle::HandleMsg(IMessagePtr& pMsg)
	{
		m_HandleCollection.Handle(pMsg);
	}

	template <class ActorType, class ValueType>
	bool ActorLite::ISystemMsgHandle::RegisterHandler(ActorType *const pActor, void (ActorType::*handler)(const ValueType &message, const Detail::Address& from), const char* pInfor /*= nullptr*/)
	{
		return m_HandleCollection.Add(pActor, handler, pInfor);
	}

}