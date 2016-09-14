#pragma once

#include <map>

#include <LibActor/Message/IMessage.h>

namespace ActorLite
{
	class IMessageHandle
	{
	public:
		IMessageHandle(){}
		virtual				~IMessageHandle(){}

		virtual bool			Handle(const IMessagePtr& message) = 0;

	private:
		IMessageHandle(const IMessageHandle &other);
		IMessageHandle &operator=(const IMessageHandle &other);
	};

}