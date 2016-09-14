#include <LibActor/BoostNet/TCPConnection.h>
#include <iostream>
namespace BoostNet
{

	TCPConnection::TCPConnection(Asio::io_service& io_service)
		:m_Socket(io_service)
	{

	}

	AsioTCP::socket& TCPConnection::Socket()
	{
		return m_Socket;
	}

	void TCPConnection::Close()
	{
		boost::system::error_code ec;
		m_Socket.shutdown(AsioTCP::socket::shutdown_both, ec);
		if (ec)
		{
			std::cout << ec.message() << std::endl;
		}
		ec.clear();
		m_Socket.close(ec);
		if (ec)
		{
			std::cout << ec.message() << std::endl;
		}
	}

}
