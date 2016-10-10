#pragma once
#include "IBase.h"

namespace Data
{

	class IDataAdapter
	{
	public:

		virtual bool	Load(uint64_t iKey, ValueMap& mValue) = 0;

		virtual bool	Load(uint64_t iKey, uint64_t iIndex, ValueMap& mValue) = 0;

		virtual void	Save(uint64_t iKey, ValueMap& mValue) = 0;

		virtual void	Save(uint64_t iKey, uint64_t iIndex, ValueMap& mValue) = 0;

		virtual void	SetValue(uint64_t iKey, IValue* pValue) = 0;

		virtual void	SetValue(uint64_t iKey, uint64_t iIndex, IValue* pValue) = 0;

		virtual void	RegistStruct(IStruct* pIStruct) = 0;

		virtual void	UnRegistStruct(IStruct* pIStruct) = 0;

		virtual std::list<uint64_t> GetAllKey(uint64_t i64Key, uint32_t iRediskey) = 0;

		virtual void	AddRepeated(uint64_t i64Key, uint64_t i64Index, uint32_t iRedisKey) = 0;

		virtual void	RemoveRepeated(uint64_t i64Key, uint64_t i64Index, uint32_t iRedisKey) = 0;

		virtual	void	ValueChange(uint64_t iKey, uint32_t iRedisKey) {}

		bool			IsRepeated() { return false; }
	};

}