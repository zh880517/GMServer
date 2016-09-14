#include <LibActor/BoostNet/TCPServer.h>
#include <LibActor/BoostNet/TCPSession.h>
#include <LibActor/BoostNet/IoServicePool.h>
namespace BoostNet
{

	TCPServer::TCPServer(IOServicePoolPtr& pIOServicePool, uint32_t iPort)
		:  m_pIOServicePool(pIOServicePool)
	{
		const AsioTCP::endpoint endpoint(AsioTCP::v4(), iPort);
		m_pAcceptor = new AsioTCP::acceptor(m_pIOServicePool->GetIOService(), endpoint);
		StartAccept();
	}

	TCPServer::~TCPServer()
	{

	}

	void TCPServer::StartAccept()
	{
		m_NewConnect.reset(new TCPSession(m_pIOServicePool->GetIOService()));
		AsioTCP::acceptor* pAcceptor = reinterpret_cast<AsioTCP::acceptor*>(m_pAcceptor);
		if (pAcceptor != nullptr)
		{
			pAcceptor->async_accept(m_NewConnect->Socket(), [this](boost::system::error_code ec)
			{
				if (!ec)
				{
					m_NewConnect->OnConnect();
				}
				StartAccept();
			});
		}
	}

}
