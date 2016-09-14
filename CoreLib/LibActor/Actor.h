#pragma once
#include <LibActor/Address.h>
#include <LibActor/Name.h>
#include <LibActor/Message/HandlerCollection.h>


namespace ActorLite
{
	class Mailbox;
	class Framework;
	class Actor
	{
		friend class Mailbox;
	public:
		explicit					Actor(const Detail::Name& name, Framework* pFramework, const Detail::Address& address = Detail::Address(0));
		virtual						~Actor();

		const	Detail::Address&	GetAddress(){ return m_Address; }

		//进入消息循环的时候调用,可以继承实现一些特殊处理，线程安全的
		virtual			void		OnBeginMsgHandle(){}

		//结束消息循环的时候调用,可以继承实现一些特殊处理，线程安全的
		virtual			void		OnEndMsgHandle(){}

		virtual	const std::string	UnSerialize(){ return ""; }

		virtual			void		Serialize(const std::string& /*strData*/){}

	protected:
		template <class ActorType, class ValueType>
		inline				bool	RegisterHandler(ActorType *const pActor, void (ActorType::*handler)(const ValueType &message, const Detail::Address& from), const char* pInfor = nullptr);

		template <class ValueType>
		inline				bool	SendValueMsg(const ValueType &value, const Detail::Address &address, bool bUdp = false) const;
							bool	SendMsg(IMessagePtr& pMsg, const Detail::Address& address)const;

		template <class ValueType>
		inline			IMessagePtr		NewMessage(bool bUdp = false);

	private:
		Detail::Address			m_Address;
		Framework*				m_pFramework;
		HandlerCollection		m_HandleCollection;//消息处理
	private:
		// Actors are non-copyable.
		Actor(const Actor &other);
		Actor &operator=(const Actor &other);

	};

	//--------------------------------------------------------------------------------------------------------------------//


	template <class ValueType>
	IMessagePtr ActorLite::Actor::NewMessage(bool bUdp)
	{
		return std::make_shared< ValueMessage<ValueType> >(m_Address, bUdp);
	}

	template <class ValueType>
	bool ActorLite::Actor::SendValueMsg(const ValueType &value, const Detail::Address &address, bool bUdp) const
	{
		IMessagePtr pMsg = std::make_shared< ValueMessage<ValueType> >(m_Address, value, bUdp);
		if (pMsg)
		{
			return SendMsg(pMsg, address);
		}
		return false;
	}

	template <class ActorType, class ValueType>
	bool ActorLite::Actor::RegisterHandler(ActorType *const pActor, void (ActorType::*handler)(const ValueType &message, const Detail::Address& from), const char* pInfor)
	{
		return m_HandleCollection.Add(pActor, handler, pInfor);
	}

	template <class ValueType>
	ValueType& Msg_Cast(IMessagePtr& pMsg)
	{
		ValueMessage<ValueType>* typedMessage = reinterpret_cast< ValueMessage<ValueType> *>(message.get());
		ACTOR_ASSERT(typedMessage != nullptr);
		return typedMessage->Value();
	}
}
