#pragma once
#include <memory>
#include "BoostHead.h"

namespace BoostNet
{
	class IOServicePool;

	class TCPConnection
	{
	public:
		TCPConnection(Asio::io_service& io_service);
		~TCPConnection() = default;

		using Ptr = std::shared_ptr<TCPConnection>;

		template<class HeadHandle>
		void	ReadHead(char* pData, uint32_t iLen, HeadHandle& handle);

		template<class BodyHandle>
		void	ReadBody(char* pData, uint32_t iLen, BodyHandle& handle);

		template<class WriteHandle>
		void	WriteData(const char* pData, uint32_t iLen, WriteHandle& handle);

		AsioTCP::socket& Socket();

		void	Close();

	private:
		AsioTCP::socket	m_Socket;
	};

	template<class WriteHandle>
	void TCPConnection::WriteData(const char* pData, uint32_t iLen, WriteHandle& handle)
	{
		Asio::async_write(m_Socket, Asio::buffer((const void*)pData, iLen), handle);
	}

	template<class BodyHandle>
	void TCPConnection::ReadBody(char* pData, uint32_t iLen, BodyHandle& handle)
	{
		Asio::async_read(m_Socket, Asio::buffer((void*)pData, iLen), handle);
	}

	template<class HeadHandle>
	void TCPConnection::ReadHead(char* pData, uint32_t iLen, HeadHandle& handle)
	{
		Asio::async_write(m_Socket, Asio::buffer((void*)pData, iLen), handle);
	}

}

