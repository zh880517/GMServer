#pragma once
#include <map>
#include "SyncValue.h"

namespace Data
{
	class IValue
	{
	public:
		virtual void			Sync(const char* pStr) {}
		virtual std::string		ToString()const = 0;
		virtual uint32_t		KeyField()const = 0;
		virtual	bool			IsConst()const = 0;
	};

	using ValueMap = std::map<uint32_t, IValue*>;

	struct IStruct;
	struct IRepeated
	{
	public:
		virtual uint32_t	Key()const = 0;
		virtual void		Load() = 0;
		virtual	void		Save() = 0;
		virtual	void		AddIndex(uint64_t iIndex) = 0;
		virtual	void		RemoveIndex(uint64_t iIndex) = 0;
		virtual	void		Sync(const SyncRepeated& stRepeated) = 0;
		virtual	bool		Load(uint64_t iIndex, ValueMap& mValue) = 0;
		virtual void		Save(uint64_t iIndex, ValueMap& mValue) = 0;
		virtual	void		SetValue(uint64_t iIndex, IValue* value) = 0;

		virtual	void		ValueChange(uint64_t, uint32_t) {}

		void	RegistStruct(IStruct*) {}
		void	UnRegistStruct(IStruct*) {}

		bool	IsRepeated() { return true; }
	};

	struct IStruct
	{
	public:
		virtual bool	Load() = 0;

		void SetKey(uint64_t iKey) { m_i64Key = iKey; }

		uint64_t GetKey()const { return m_i64Key; }

		void	Sync(const SyncValue& stValue)
		{
			if (m_mValue.empty())
				return;
			for (auto it : stValue.mValue)
			{
				auto itFind = m_mValue.find(it.first);
				if (itFind != m_mValue.end() && itFind->second->IsConst())
				{
					itFind->second->Sync(it.second.c_str());
				}
			}
		}
		void	Sync(const SyncStruct& stStruct)
		{
			Sync(stStruct.stValue);
			for (auto it : stStruct.mRepeated)
			{
				auto itFind = m_mRepeated.find(it.first);
				if (itFind != m_mRepeated.end())
				{
					itFind->second->Sync(it.second);
				}
			}
		}

		void	RegistValue(IValue* value)
		{
			m_mValue.insert(std::make_pair(value->KeyField(), value));
		}

		void	RegisRepeated(IRepeated* repeated)
		{
			m_mRepeated.insert(std::make_pair(repeated->Key(), repeated));
		}

		void	SetNotifyState(bool bNotify) { m_bNotifyValueChange = bNotify; }

	protected:
		uint64_t							m_i64Key = 0;
		bool								m_bNotifyValueChange = false;
		ValueMap							m_mValue;
		std::map<uint64_t, IRepeated*>		m_mRepeated;
	};

}