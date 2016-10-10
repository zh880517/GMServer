#pragma once

#include <map>
#include <list>
#include "IBase.h"

namespace Data
{


	template<class ValueType, class DataHandle, uint32_t _key>
	class TBaseRepeat : public IRepeated
	{
	public:
		TBaseRepeat() : m_pHandle(nullptr)
		{}

		TBaseRepeat(const TBaseRepeat& other) = default;
		~TBaseRepeat() = default;

		void	SetDataHandle(DataHandle* pHandle)
		{
			m_pHandle = pHandle;
			if (m_pHandle != nullptr)
				m_pHandle->RegisRepeated(this);
		}

		const ValueType*	Get(uint64_t iIndex) const
		{
			auto itFind = m_mValue.find(iIndex);
			if (itFind != m_mValue.cend())
				return &(itFind->second);
			return nullptr;
		}

		template<typename T>
		void	ForEach(T& t)
		{
			for (auto& it : m_mValue)
			{
				if (!t(t.first, it.second))
					break;
			}
		}

		uint32_t Size() const { return (uint32_t)m_mValue.size(); }

		virtual uint32_t Key() const override
		{
			return _key;
		}

		void	SetValue(uint64_t iIndex, IValue* value)
		{
			if (m_pHandle != nullptr)
				m_pHandle->SetValue(iIndex, value);
		}

		bool	Load(uint64_t iIndex, ValueMap& mValue)
		{
			if (m_pHandle == nullptr)
				return false;
			return m_pHandle->Load(iIndex, mValue);
		}

		void	Save(uint64_t iIndex, ValueMap& mValue)
		{
			if (m_pHandle != nullptr)
				m_pHandle->Save(iIndex, mValue);
		}

	protected:

		virtual void Load() override
		{
			std::list<uint64_t> listKey = m_pHandle->GetAllKey(_key);
			for (auto it : listKey)
			{
				ValueType stData;
				stData.SetKey(it);
				stData.SetDataHandle(this);
				if (stData.Load())
				{
					auto itInsert = m_mValue.insert(std::make_pair(it, stData));
					if (itInsert.second)
					{
						itInsert.first->second.SetDataHandle(this);
					}
				}
			}
		}

		virtual void AddIndex(uint64_t iIndex) override
		{
			ValueType& stData = m_mValue[iIndex];
			stData.SetKey(iIndex);
			stData.SetDataHandle(this);
			if (stData.Load())
			{
				auto itInsert = m_mValue.insert(std::make_pair(iIndex, stData));
				if (itInsert.second)
				{
					itInsert.first->second.SetDataHandle(this);
				}
			}
		}

		virtual void RemoveIndex(uint64_t iIndex) override
		{
			m_mValue.erase(iIndex);
		}

		virtual void Sync(const SyncRepeated& stRepeated) override
		{
			for (auto& it : stRepeated.vChange)
			{
				if (it.second)
				{
					AddIndex(it.first);
				}
				else
				{
					RemoveIndex(it.first);
				}
			}
			for (auto it : stRepeated.mValue)
			{
				auto itFind = m_mValue.find(it.first);
				if (itFind != m_mValue.end())
					itFind->second.Sync(it.second);
			}
		}

		virtual void	Save() override
		{
			for (auto& it : m_mValue)
			{
				it.second.Save();
			}
		}

	protected:
		DataHandle*									m_pHandle;
		std::map<uint64_t, ValueType>				m_mValue;
	};

	template<class ValueType, class DataHandle, uint32_t _key>
	class TRepeated : public TBaseRepeat<ValueType, DataHandle, _key>
	{
	public:
		TRepeated() = default;
		TRepeated(const TRepeated& other) = default;
		~TRepeated() = default;

		ValueType*	Get(uint64_t iIndex)
		{
			auto itFind = m_mValue.find(iIndex);
			if (itFind != m_mValue.end())
				return &(itFind->second);
			return nullptr;
		}

		bool	Add(ValueType& stData, uint64_t i64Index)
		{
			auto it = m_mValue.insert(std::make_pair(stData.GetKey(), stData));
			if (!it.second)
				return false;
			it.first->second.SetKey(i64Index);
			it.first->second.SetDataHandle(this);
			it.first->second.Save();
			if (m_pHandle != nullptr)
				m_pHandle->AddRepeated(i64Index, _key);
			return true;
		}

		void	Remove(uint64_t i64Index)
		{
			m_mValue.erase(i64Index);
			m_pHandle->RemoveRepeated(i64Index, _key);
		}
	};

}