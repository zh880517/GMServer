#include <LibActor/Actor.h>
#include <LibActor/Framework.h>
namespace ActorLite
{


	Actor::Actor(const Detail::Name& name, Framework* pFramework, const Detail::Address& address)
		: m_pFramework(pFramework)
		, m_Address(address)
	{
		if (address == Detail::Address(0))
		{
			m_pFramework->RegisterActor(this, name);
		}
	}


	Actor::~Actor()
	{
		if (m_pFramework != nullptr)
		{
			m_pFramework->UnRegisterActor(this);
		}
	}

	bool Actor::SendMsg(IMessagePtr& pMsg, const Detail::Address& address) const
	{
		return m_pFramework->SendMsg(pMsg, address);
	}

}
