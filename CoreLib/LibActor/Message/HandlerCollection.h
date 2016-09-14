#pragma once

#include <Utils/AssertUtils.h>
#include <LibActor/Message/MessageHandler.h>

namespace ActorLite
{

	class HandlerCollection
	{
	public:
		HandlerCollection(){}
		~HandlerCollection(){ Clear(); }

		template <class ActorType, class ValueType>
		inline bool Add(ActorType* pActor, void (ActorType::*handler)(const ValueType &message, const Detail::Address& from), const char* pInfor)
		{
			ASSERT_UTILS(pActor != nullptr);
			ASSERT_UTILS(handler != nullptr);
			MessageHandler<ActorType, ValueType>* pHandler = new MessageHandler < ActorType, ValueType >(pActor, handler, pInfor);
			uint32_t nHashValue = MessageTraits<ValueType>::TYPE_ID;
			HandleMap::iterator it = m_mapHandles.find(nHashValue);
			if (it == m_mapHandles.end())
			{
				std::pair< std::map< uint32_t, IMessageHandle* >::iterator, bool > ret;
				ret = m_mapHandles.insert(std::make_pair(nHashValue, pHandler));
				if (ret.second)
				{
					return true;
				}
			}
			ASSERT_UTILS_MSG(false, "Message Handler has been existing!");
			return false;
		}

		inline bool	Handle(const IMessagePtr& message)
		{
			uint32_t nMsgType = message->TypeID();
			HandleMap::iterator it = m_mapHandles.find(nMsgType);
			if (it != m_mapHandles.end())
			{
				it->second->Handle(message);
				return true;
			}
			return false;
		}

		void		Clear()
		{
			for (HandleMap::iterator it = m_mapHandles.begin(); it != m_mapHandles.end(); ++it)
			{
				if (it->second != nullptr)
				{
					delete it->second;
				}
			}
			m_mapHandles.clear();
		}
	private:
		using HandleMap = std::map<uint32_t, IMessageHandle*>;
		HandleMap	m_mapHandles;
	};


}