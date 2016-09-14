#pragma once

#include <list>
#include <atomic>
#include <LibActor/Message/IMessage.h>
#include <LibActor/Address.h>
#include <LibActor/SharePtrDef.h>
namespace ActorLite
{
	class Framework;
	class EndPoint : public std::enable_shared_from_this<EndPoint>
	{
	public:
		EndPoint(Framework& framework, IOServicePoolPtr& pIOService);
		~EndPoint();

		bool				Connect(const std::string& strIP, const std::string& strPort);

		bool				SendMsg(IMessagePtr& pMsg, const Detail::Address& adress);
		bool				SendMsg(IMessagePtr& pMsg, std::list<Detail::Address>& listAdress);

		void				DisConnect();

	private:
		void				DoConnect();
		void				OnDisconnect();

		void				OnNewMsg(BoostNet::NetPacket* pNewPacket);
	private:
		std::string				m_strIP;
		std::string				m_strPort;
		Framework&				m_rfFramwork;
		IOServicePoolPtr		m_pIOPool;
		TCPSessionPtr			m_pSession;
		std::atomic_bool		m_bShutDown;
	};

}