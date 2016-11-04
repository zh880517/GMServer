#pragma once

#include <list>
#include <functional>
#include <mutex>
#include <LibActor/BoostNet/TCPConnection.h>
#include <LibActor/BoostNet/NetPacket.h>
#include <LibActor/SharePtrDef.h>

namespace BoostNet
{

	/*
	通用的一个TCP连接类，可以用来保存服务端监听的链接，也可以用来主动向服务端发起请求
	*/
	class TCPSession : public std::enable_shared_from_this<TCPSession>
	{
	public:
		TCPSession(Asio::io_service& io_service);
		~TCPSession();

		void				OnConnect();

		void				ConnectServer(AsioTCP::resolver::iterator endpoint_iterator, const std::function<void(bool)>& onConnetResult);

		void				SetDisconnctHandle(const std::function<void()> & handle);

		void				SetOnMsgHandle(const std::function<void(NetPacket*)>& handle);

		void				SendPacket(NetPacketPtr& pPacket);

		void				SendPacketList(std::list<NetPacketPtr>& listPacket);

		void				DisConnect();

		AsioTCP::socket&	Socket();

	private:
		void				ReadHead();

		void				ReadBody();

		void				DoSend();

		void				CancelDeadLineTimer();

		void				SetDeadLineTimer(uint32_t iSecond);
	private:
		TCPConnection						m_Connection;
		uint16_t							m_iPackLen;
		NetPacket*							m_pReadMsg;
		std::mutex							m_MsgMutex;
		std::list<NetPacketPtr>				m_listSend;

		std::function<void()>				m_OnDisconnectHandle;
		std::function<void(NetPacket*)>		m_OnMsgHandle;
		Asio::deadline_timer				m_DeadLineTimer;
	};

}
