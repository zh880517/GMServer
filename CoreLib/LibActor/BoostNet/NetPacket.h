#pragma once
#include <memory>
#include <string>

namespace BoostNet
{
	class NetPacket
	{
	public:
		explicit NetPacket(uint16_t iPacketSize);
		explicit NetPacket(const std::string& strMsg);
		~NetPacket();

		//返回数据部分
		char*			GetDataPtr();

		//返回缓冲区
		const char*		GetBuffPtr();

		//缓冲区总大小，减去两个字节就是实际消息的大小了
		uint16_t		GetSize();

	private:
		std::string	m_strMsg;
	};
}
