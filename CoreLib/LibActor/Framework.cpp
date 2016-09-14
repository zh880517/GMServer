#include <LibActor/Framework.h>
#include <LibActor/IScheduler.h>
#include <LibActor/Actor.h>
#include <LibActor/EndPoint.h>
#include <LibActor/BoostNet/IoServicePool.h>
namespace ActorLite
{
	Framework::Framework(IScheduler* pScheduler)
		: m_pScheduler(pScheduler)
		, m_Address(0)
		, m_bDestory(false)
	{
		m_MailBoxes.Allocate();//占用索引为0的MailBox，用来处理系统消息
		
	}


	Framework::~Framework()
	{
	}

	bool Framework::Init(/*FrameworkConfig & stConfig*/)
	{
// 		if (stConfig.strChannleIP.empty() || stConfig.strChannlePort.empty())
// 		{
// 			return false;
// 		}
// 		m_pIoServicePool = std::make_shared<BoostNet::IOServicePool>(stConfig.iThreadNum);
// 		m_pIoServicePool->Run();
// 		m_Address.m_Detail.m_Channel = stConfig.iChannelID;
// 		m_Address.m_Detail.m_Group = stConfig.iGroupID;
// 		m_Address.m_Detail.m_Process = stConfig.iProcessID;
// 		m_pEndPoint = std::make_shared<EndPoint>(*this, m_pIoServicePool);
// 		return m_pEndPoint->Connect(stConfig.strChannleIP, stConfig.strChannlePort);
		return false;
	}

	bool Framework::SendInternal(IMessagePtr& pMsg, const Detail::Address& adress)
	{
		ASSERT_UTILS(m_Address.m_Brief.mProcess == adress.m_Brief.mProcess);
		Mailbox& refMailBox(m_MailBoxes.GetEntry(adress.m_Detail.m_Index));
		return refMailBox.PushMsg(pMsg);
	}

	void Framework::RegisterActor(Actor* pActor, const Detail::Name& name)
	{
		const uint32_t nNewIndex = m_MailBoxes.Allocate();
		ASSERT_UTILS(nNewIndex != -1);
		Mailbox& refMailBox = m_MailBoxes.GetEntry(nNewIndex);
		ASSERT_UTILS(refMailBox.GetActor() == nullptr);
		ASSERT_UTILS(refMailBox.GetAddress().m_Uint32 == 0);
		refMailBox.SetScheduler(m_pScheduler);
		refMailBox.SetName(name);
		refMailBox.SetAddress(Detail::Address(m_Address.m_Brief.mProcess, nNewIndex));
		refMailBox.RegisterActor(pActor);
	}

	void Framework::RunOnce()
	{
		m_pScheduler->RunOnce();
		//系统消息处理
// 		if(m_pSystemHandle != nullptr)
// 		{
// 			m_ReciveMutex.Lock();
// 			std::swap(m_listHandle, m_listRecive);
// 			m_ReciveMutex.UnLock();
// 			while (!m_listHandle.empty())
// 			{
// 				IMessagePtr pMsg = m_listHandle.front();
// 				m_listHandle.pop_front();
// 			}
// 		}
	}

	bool Framework::SendEndPoint(IMessagePtr& pMsg, const Detail::Address& adress)
	{
		return m_pEndPoint->SendMsg(pMsg, adress);
	}

	bool Framework::SendEndPoint(IMessagePtr& pMsg, std::list<Detail::Address>& listAdress)
	{
		return m_pEndPoint->SendMsg(pMsg, listAdress);
	}

	bool Framework::SendMsg(IMessagePtr& pMsg, const Detail::Address& adress)
	{
		if (adress.m_Brief.mProcess == m_Address.m_Brief.mProcess)
		{
			if (adress.m_Detail.m_Index == 0)
			{
				SystemMsg(pMsg); 
			}
			else
			{
				SendInternal(pMsg, adress);
			}
		}
		else
		{
			SendEndPoint(pMsg, adress);
		}
		return true;
	}

	bool Framework::SendMsg(IMessagePtr& pMsg, std::list<Detail::Address>& listAdress)
	{
		std::list<Detail::Address> listEndPoint;
		for (auto it : listAdress)
		{
			if (it.m_Brief.mProcess == m_Address.m_Brief.mProcess)
			{
				if (it.m_Detail.m_Index == 0)
				{
					SystemMsg(pMsg);
				}
				else
				{
					SendInternal(pMsg, it);
				}
			}
			else
			{
				listEndPoint.push_back(it);
			}
		}
		if (!listEndPoint.empty())
		{
			return SendEndPoint(pMsg, listEndPoint);
		}
		return true;
	}

	void Framework::UnRegisterActor(Actor *const pActor)
	{
		const Detail::Address address(pActor->GetAddress());
		Mailbox& refMailBox(m_MailBoxes.GetEntry(address.m_Detail.m_Index));
		refMailBox.UnRegisterActor();
	}

	void Framework::Destroy()
	{
		m_pEndPoint->DisConnect();
		m_pIoServicePool->Stop();
		if (m_pScheduler != nullptr)
		{
			m_pScheduler->Destroy();
		}
		uint32_t iMaxIndex = m_MailBoxes.GetMaxIndex();
		for (size_t i = 1; i < iMaxIndex; ++i)
		{
			Mailbox& refMailBox = m_MailBoxes.GetEntry(i);
			refMailBox.DebugOut();
		}
	}

	bool Framework::SystemMsg(IMessagePtr& pMsg)
	{
		m_ReciveMutex.lock();
		m_listRecive.push_back(pMsg);
		m_ReciveMutex.unlock();
		return true;
	}


}