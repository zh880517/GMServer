#include <LibActor/EndPoint.h>
#include <iostream>
#include <LibActor/Framework.h>
#include <LibActor/BoostNet/TCPSession.h>
#include <LibActor/BoostNet/IoServicePool.h>
#include <LibActor/BoostNet/NetPacket.h>
namespace ActorLite
{

	EndPoint::EndPoint(Framework& framework, IOServicePoolPtr& pPool)
		: m_rfFramwork(framework)
		, m_pIOPool(pPool)
	{
		m_pSession = std::make_shared< BoostNet::TCPSession>(m_pIOPool->GetIOService());
		auto self(shared_from_this());
		m_pSession->SetDisconnctHandle([this, self]() {
			OnDisconnect();
		});
		m_bShutDown.store(false);
	}

	EndPoint::~EndPoint()
	{
	}

	bool EndPoint::Connect(const std::string & strIP, const std::string & strPort)
	{
		m_strIP = strIP;
		m_strPort = strPort;

		DoConnect();
		return true;
	}

	bool EndPoint::SendMsg(IMessagePtr& pMsg, const Detail::Address& adress)
	{
		return true;
	}

	bool EndPoint::SendMsg(IMessagePtr& pMsg, std::list<Detail::Address>& listAdress)
	{
		return true;
	}

	void EndPoint::DisConnect()
	{
		m_pSession->DisConnect();
		m_bShutDown.store(true);
	}

	void EndPoint::DoConnect()
	{
		AsioTCP::resolver resolver(m_pIOPool->GetIOService());
		auto endpoint_iterator = resolver.resolve({ m_strIP, m_strPort });
		auto self(shared_from_this());
		std::function<void(bool)> function = [this, self](bool bSucc) {
			if (!bSucc && !m_bShutDown.load())
			{
				std::cout << "Connect to channel Server IP = " << m_strIP << ", Port = " << m_strPort << " Fail!" << std::endl;
				DoConnect();
			}
			if (bSucc && !m_bShutDown.load())
			{
				std::function<void(BoostNet::NetPacket*)> funOnMsg = std::bind(&EndPoint::OnNewMsg, self, std::placeholders::_1);
				m_pSession->SetOnMsgHandle(funOnMsg);
			}
		};
		m_pSession->ConnectServer(endpoint_iterator, function);
	}

	void EndPoint::OnDisconnect()
	{
		if (!m_bShutDown.load())
		{
			std::cout << "Disconnect to channel Server IP = " << m_strIP << ", Port = " << m_strPort << " Fail!" << std::endl;
			DoConnect();
		}
	}

	void EndPoint::OnNewMsg(BoostNet::NetPacket* pNewPacket)
	{
// 		NetPacketToMsg tranPack(pNewPacket);
// 		IMessagePtr pMsg = tranPack.Msg();
// 		const std::vector<uint32_t>& vReciver = tranPack.Reciver();
// 		uint32_t iProcessID = tranPack.ProcessID();
// 		for (auto iIndex : vReciver)
// 		{
// 			Detail::Address address(iProcessID, iIndex);
// 			m_rfFramwork.SendMsg(pMsg, address);
// 		}
	}

}

