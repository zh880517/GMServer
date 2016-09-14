#pragma once

#include <LibActor/Message/IMessage.h>

namespace ActorLite
{
	template <class ValueType>
	class ValueMessage : public IMessage
	{
	public:
		explicit						ValueMessage(const Detail::Address& from, bool bUdp):IMessage(from, false, bUdp){ }
		explicit						ValueMessage(const Detail::Address& from, ValueType value, bool bUdp) :IMessage(from, false, bUdp), m_Value(value){ }


				ValueType&				Value(){ return m_Value; }

		const	ValueType&				Value()const { return m_Value; }

		const char*						TypeName() const { return MessageTraits<ValueType>::TYPE_NAME; }

		virtual uint32_t				TypeID() const override { return MessageTraits<ValueType>::TYPE_ID; }

	private:
		ValueType						m_Value;
	private:
		ValueMessage(const ValueMessage &other);
		ValueMessage &operator=(const ValueMessage &other);
	public:
		~ValueMessage(){}


	};

}