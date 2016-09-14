#pragma once

#include <Utils/AssertUtils.h>
#include <LibActor/Message/IMessageHandle.h>
#include <LibActor/Message/ValueMessage.h>

namespace ActorLite
{
	template <class ActorType, class ValueType>
	class MessageHandler : public IMessageHandle
	{
	public:
		using HandlerFunction =  void (ActorType::*)(const ValueType &message, const Detail::Address& from);


		inline explicit				MessageHandler(ActorType* pActor, HandlerFunction function, const char* pInfor)
			: m_Actor(pActor)
			, m_HandlerFunction(function)
			, m_pDebugInfor(pInfor){}

		//消息处理
		virtual bool				Handle(const IMessagePtr&  message)
		{
			ASSERT_UTILS(m_Actor);
			ASSERT_UTILS(m_HandlerFunction);

			const ValueMessage<ValueType> *const typedMessage = static_cast<const ValueMessage<ValueType> *>(message.get());
			if (nullptr != typedMessage)
			{
				(m_Actor->*m_HandlerFunction)(typedMessage->Value(), typedMessage->From());
				return true;
			}
			return false;
		}

	private:
		const HandlerFunction		m_HandlerFunction;
		ActorType*					m_Actor;
		const char*					m_pDebugInfor;//注册的函数名，方便调试输出
	};
}