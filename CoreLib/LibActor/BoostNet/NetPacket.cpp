#pragma once
#include <LibActor/BoostNet/NetPacket.h>

namespace BoostNet
{

	NetPacket::NetPacket(uint16_t iSize)
		: m_strMsg(iSize + 3, 0)
	{
		uint16_t* pLen = (uint16_t*)(&(m_strMsg[0]));
		*pLen = iSize;
	}

	NetPacket::NetPacket(const std::string& strMsg)
	{
		uint16_t iLen = (uint16_t)strMsg.size();
		m_strMsg.append((char*)&iLen, 2);
		m_strMsg.append(strMsg);
	}

	NetPacket::~NetPacket()
	{
	}

	char* NetPacket::GetDataPtr()
	{
		char* pRead = &(m_strMsg[0]);
		return  pRead + 2;
	}

	uint16_t NetPacket::GetSize()
	{
		return (uint16_t)m_strMsg.size();
	}

	const char* NetPacket::GetBuffPtr()
	{
		return &(m_strMsg[0]);
	}


}
