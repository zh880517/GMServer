#pragma once
#include <memory>
#include <string>

namespace BoostNet
{

	/*
	网络数据包类
	数据包存储结构: |---后续数据包大小(两个字节)--|---包类型(一个字节)---|----包内容----|
	客户端与服务器、服务器与服务器之间通信的实际包头都不一样，所以采用这种简单的数据包格式
	因为可以直接从连接上来区分数据包类型，所以只用保存包的长度来处理数据包粘包
	剩余的包内容，一般使用Sdp协议格式存储,包括广播包都可以方便的处理
	*/

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
