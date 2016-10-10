#pragma once

#include <functional>
#include "IBase.h"

namespace Data
{


	template<class DataHandle>
	struct TStruct : IStruct
	{
		TStruct() : m_pDataHanle(nullptr)
		{
			OnValueChange = [](uint32_t) {};
			OnRepeatedValueChange = [](uint64_t, uint32_t) {};
			OnRepeatedAdd = [](uint64_t, uint64_t) {};
			OnRepeatedRemove = [](uint64_t, uint64_t) {};
		}

		TStruct(const TStruct& other) = default;
		~TStruct()
		{
			if (m_pDataHanle != nullptr)
				m_pDataHanle->UnRegistStruct(this);
		}
		void	SetDataHandle(DataHandle* pDataHanle)
		{
			m_pDataHanle = pDataHanle;
			if (m_pDataHanle != nullptr)
			{
				m_pDataHanle->RegistStruct(this);
			}
		}

		void	SetValue(IValue* value)
		{
			if (m_pDataHanle != nullptr)
				m_pDataHanle->SetValue(m_i64Key, value);
		}

		void	SetValue(uint64_t iIndex, IValue* value)
		{
			if (m_pDataHanle != nullptr)
				m_pDataHanle->SetValue(m_i64Key, iIndex, value);
		}

		void	AddRepeated(uint64_t i64Index, uint32_t iRedisKey)
		{
			if (m_pDataHanle != nullptr)
				m_pDataHanle->AddRepeated(m_i64Key, i64Index, iRedisKey);
		}

		void	RemoveRepeated(uint64_t i64Index, uint32_t iRedisKey)
		{
			if (m_pDataHanle != nullptr)
				m_pDataHanle->RemoveRepeated(m_i64Key, i64Index, iRedisKey);
		}

		void	Save(ValueMap& mValue)
		{
			if (m_pDataHanle != nullptr)
				m_pDataHanle->Save(m_i64Key, mValue);
		}

		void	ValueChange(uint32_t iKeyFiled)
		{
			if (!m_bNotifyValueChange)
				return;
			if (m_pDataHanle->IsRepeated())
			{
				m_pDataHanle->ValueChange(m_i64Key, iKeyFiled);
			}
			else
			{
				OnValueChange(iKeyFiled);
			}
		}

		bool	Load(uint64_t iIndex, ValueMap& mValue)
		{
			if (m_pDataHanle == nullptr)
				return false;
			return m_pDataHanle->Load(m_i64Key, iIndex, mValue);
		}

		std::list<uint64_t> GetAllKey(uint32_t key)
		{
			if (m_pDataHanle == nullptr)
				return std::list<uint64_t>();
			return m_pDataHanle->GetAllKey(m_i64Key, key);
		}

		virtual bool Load() override
		{
			if (m_pDataHanle == nullptr)
				return false;

			bool bNotify = m_bNotifyValueChange;
			m_bNotifyValueChange = false;
			bool bOK = true;
			if (!m_mValue.empty() && !m_pDataHanle->Load(m_i64Key, m_mValue))
				bOK = false;
			for (auto& it : m_mRepeated)
			{
				it.second->Load();
			}
			m_bNotifyValueChange = bNotify;
			return bOK;
		}

		void Save()
		{
			if (m_pDataHanle == nullptr)
				return;
			m_pDataHanle->Save(m_i64Key, m_mValue);
			for (auto& it : m_mRepeated)
			{
				it.second->Save();
			}
		}

		void Save(uint64_t iIndex, ValueMap& mValue)
		{
			m_pDataHanle->Save(m_i64Key, iIndex, mValue);
		}


	public:

		std::function<void(uint32_t)> OnValueChange;
		std::function<void(uint64_t, uint32_t)> OnRepeatedValueChange;
		std::function<void(uint64_t, uint64_t)>	OnRepeatedAdd;
		std::function<void(uint64_t, uint64_t)> OnRepeatedRemove;

	protected:
		DataHandle*		m_pDataHanle;
	};

}