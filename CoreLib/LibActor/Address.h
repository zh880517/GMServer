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
			uint32_t m_Group : 7;//Ⱥ��ID��0���ã���Ϊ����Ĺ���Actor���� ����Ⱥ��ID 1-127
			uint32_t m_Channel : 7;//ת��������ID��0����Ⱥ������ʹ�ã�����ת��������ID 1-127
			uint32_t m_Process : 5;//����ID,0����������ת�������������ý���ID 1-31
			uint32_t m_Index : 13;//�ڲ�����ID��0�����������̣��������ݲ�ID 1-8191
		}m_Detail;

		struct
		{
			uint32_t mProcess : 19;
			uint32_t m_Index : 13;
		}m_Brief;
	};
}