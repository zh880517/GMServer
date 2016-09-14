#include <LibActor/BoostNet/TCPSession.h>

namespace BoostNet
{

	TCPSession::TCPSession(Asio::io_service& io_service)
		: m_Connection(io_service)
		, m_DeadLineTimer(io_service)
	{
	}


	TCPSession::~TCPSession()
	{
	}

	void TCPSession::OnConnect()
	{
		ReadHead();
		SetDeadLineTimer(3);
	}


	void TCPSession::ReadHead()
	{
		auto self(shared_from_this());
		m_Connection.ReadHead((char*)(&m_iPackLen), sizeof(uint16_t), [this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				ReadBody();
			}
			else
			{
				//错误处理，断开链接
				m_Connection.Close();
				m_OnDisconnectHandle();
			}
			CancelDeadLineTimer();
		});
	}

	void TCPSession::ReadBody()
	{
		m_pReadMsg = new NetPacket(m_iPackLen);

		auto self(shared_from_this());
		m_Connection.ReadBody(m_pReadMsg->GetDataPtr(), m_iPackLen, [this, self](boost::system::error_code ec, std::size_t /*length*/)
		{
			if (!ec)
			{
				m_OnMsgHandle(m_pReadMsg);
				m_pReadMsg = nullptr;
				ReadHead();
			}
			else
			{
				//错误处理，断开链接
				m_Connection.Close();
				m_OnDisconnectHandle();
			}
		});
	}

	void TCPSession::SetDisconnctHandle(const std::function<void()> & handle)
	{
		m_OnDisconnectHandle = handle;
	}

	void TCPSession::SetOnMsgHandle(std::function<void(NetPacket*)>& handle)
	{
		m_OnMsgHandle = handle;
	}

	void TCPSession::DisConnect()
	{
		m_OnMsgHandle = [](NetPacket* pPack) { delete pPack; };
		m_OnDisconnectHandle = [](){};
		m_Connection.Close();
	}

	void TCPSession::SendPacket(NetPacketPtr& pPacket)
	{
		m_MsgMutex.lock();
		bool bDoSend = m_listSend.empty();
		m_listSend.push_back(pPacket);
		m_MsgMutex.unlock();
		if (bDoSend)
		{
			DoSend();
		}
	}


	void TCPSession::SendPacketList(std::list<NetPacketPtr>& listPacket)
	{
		m_MsgMutex.lock();
		bool bDoSend = m_listSend.empty();
		m_listSend.splice(m_listSend.end(), listPacket);
		m_MsgMutex.unlock();
		if (bDoSend)
		{
			DoSend();
		}
	}


	void TCPSession::DoSend()
	{
		bool bSend = false;
		NetPacketPtr pSend;
		m_MsgMutex.lock();
		{
			if (!m_listSend.empty())
			{
				pSend = m_listSend.front();
				bSend = true;
			}
			m_listSend.pop_front();
		}
		m_MsgMutex.unlock();

		if (bSend)
		{
			auto self(shared_from_this());
			m_Connection.WriteData(pSend->GetBuffPtr(), pSend->GetSize(), [this, self, pSend](boost::system::error_code ec, std::size_t /*length*/)
			{
				if (!ec)
				{
					DoSend();
				}
				else
				{
					//发送失败后，要把数据包放在队列的前面，等连接重新建立后继续发送
					m_MsgMutex.lock();
					m_listSend.push_front(pSend);
					m_MsgMutex.unlock();
					//错误处理，断开链接
					m_Connection.Close();
					m_OnDisconnectHandle();
				}
			});
		}

	}

	AsioTCP::socket& TCPSession::Socket()
	{
		return m_Connection.Socket();
	}

	void TCPSession::ConnectServer(AsioTCP::resolver::iterator endpoint_iterator, const std::function<void(bool)>& onConnetResult)
	{
		auto self(shared_from_this());
		Asio::async_connect(m_Connection.Socket(), endpoint_iterator, [this, self, onConnetResult](const boost::system::error_code& error, AsioTCP::resolver::iterator endpoint_iterator)
		{
			bool bResult = false;
			if (!error)
			{
				bResult = true;
				ReadBody();
				//如果是断线重连，可能有未发送成功的消息，需要重新发送
				DoSend();
			}
			onConnetResult(bResult);
		});
	}

	void TCPSession::CancelDeadLineTimer()
	{
		boost::system::error_code ec;
		m_DeadLineTimer.cancel(ec);
	}

	void TCPSession::SetDeadLineTimer(uint32_t iSecond)
	{
		auto self(shared_from_this());
		m_DeadLineTimer.expires_from_now(boost::posix_time::seconds(iSecond));
		m_DeadLineTimer.async_wait([this, self](const boost::system::error_code& ec)
		{
			if (!ec)
			{
				m_OnDisconnectHandle();
				DisConnect();
			}
		});
	}


}
