#pragma once
#include <string>
#include <LibActor/SharePtrDef.h>
namespace BoostNet
{

	class TCPServer
	{
	public:
		TCPServer(IOServicePoolPtr& pIOServicePool, uint32_t iPort);
		~TCPServer();

	private:
		void		StartAccept();

	private:
		IOServicePoolPtr					m_pIOServicePool;
		void*								m_pAcceptor;
		TCPSessionPtr						m_NewConnect;
	};


}

