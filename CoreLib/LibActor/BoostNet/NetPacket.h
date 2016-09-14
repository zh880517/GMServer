#pragma once
#include <memory>
#include <string>

namespace BoostNet
{

	/*
	�������ݰ���
	���ݰ��洢�ṹ: |---�������ݰ���С(�����ֽ�)--|---������(һ���ֽ�)---|----������----|
	�ͻ�������������������������֮��ͨ�ŵ�ʵ�ʰ�ͷ����һ�������Բ������ּ򵥵����ݰ���ʽ
	��Ϊ����ֱ�Ӵ����������������ݰ����ͣ�����ֻ�ñ�����ĳ������������ݰ�ճ��
	ʣ��İ����ݣ�һ��ʹ��SdpЭ���ʽ�洢,�����㲥�������Է���Ĵ���
	*/

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
