#pragma once

#include <memory>

namespace BoostNet
{
	class IOServicePool;
	class NetPacket;
	class TCPSession;
}

using IOServicePoolPtr = std::shared_ptr<BoostNet::IOServicePool>;
using NetPacketPtr = std::shared_ptr<BoostNet::NetPacket>;
using TCPSessionPtr = std::shared_ptr<BoostNet::TCPSession>;


namespace ActorLite
{
	class EndPoint;
	class IMessage;
}
using IMessagePtr = std::shared_ptr<ActorLite::IMessage>;
using EndPointPtr = std::shared_ptr<ActorLite::EndPoint>;