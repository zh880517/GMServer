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

		//�������ݲ���
		char*			GetDataPtr();

		//���ػ�����
		const char*		GetBuffPtr();

		//�������ܴ�С����ȥ�����ֽھ���ʵ����Ϣ�Ĵ�С��
		uint16_t		GetSize();

	private:
		std::string	m_strMsg;
	};
}
