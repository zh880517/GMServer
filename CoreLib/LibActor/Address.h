#pragma once
#include <stdint.h>

namespace Detail
{

	union Address
	{
		explicit Address(const uint32_t iGroupiD, const uint32_t iChannelID, const uint32_t iProcessID, const uint32_t iIndex) : m_Uint32(0)
		{
			m_Detail.m_Group = iGroupiD;
			m_Detail.m_Channel = iChannelID;
			m_Detail.m_Process = iProcessID;
			m_Detail.m_Index = iIndex;
		}

		explicit Address(const uint16_t iProcessID, const uint16_t index) : m_Uint32(0)
		{
			m_Brief.mProcess = iProcessID;
			m_Brief.m_Index = index;
		}

		explicit Address(const uint32_t& iIndex) : m_Uint32(iIndex)
		{
		}

		explicit Address() : m_Uint32(0)
		{
		}

		Address(const Address &other) : m_Uint32(other.m_Uint32)
		{
		}


		Address &operator=(const Address &other)
		{
			m_Uint32 = other.m_Uint32;
			return *this;
		}
		bool operator==(const Address &other) const
		{
			return (m_Uint32 == other.m_Uint32);
		}

		bool operator!=(const Address &other) const
		{
			return (m_Uint32 != other.m_Uint32);
		}

		bool operator<(const Address &other) const
		{
			return (m_Uint32 < other.m_Uint32);
		}

		uint32_t m_Uint32;
		struct 
		{
			uint32_t m_Group : 7;//群组ID，0不用，作为特殊的管理Actor域名 可用群组ID 1-127
			uint32_t m_Channel : 7;//转发服务器ID，0留给群组主机使用，可用转发服务器ID 1-127
			uint32_t m_Process : 5;//进程ID,0代表所属的转发服务器，可用进程ID 1-31
			uint32_t m_Index : 13;//内部索引ID，0代表所属进程，可用内容部ID 1-8191
		}m_Detail;

		struct
		{
			uint32_t mProcess : 19;
			uint32_t m_Index : 13;
		}m_Brief;
	};
}