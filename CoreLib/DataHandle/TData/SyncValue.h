#pragma once

#include <map>
#include <list>
#include <string>
#include <unordered_map>

namespace Data
{

	struct SyncValue
	{
		void	AddValue(uint32_t iKeyField, const std::string& strValue)
		{
			mValue[iKeyField] = strValue;
		}

		std::unordered_map<uint32_t, std::string>  mValue;
	};

	struct SyncRepeated
	{
		std::unordered_map<uint64_t, SyncValue>	mValue;
		//<index, add>
		std::list<std::pair<uint64_t, bool>> vChange;
	};

	struct SyncStruct
	{
		void	AddValue(uint64_t iIndex, uint32_t iKeyField, const std::string& strValue)
		{
			if (iIndex == 0)
			{
				stValue.AddValue(iKeyField, strValue);
				return;
			}
			mRepeated[iKeyField &(~0xFF)].mValue[iIndex].AddValue(iKeyField, strValue);
		}

		void	RepeatedModify(uint64_t iIndex, uint32_t iRedisKey, bool bAdd)
		{
			mRepeated[iRedisKey &(~0xFF)].vChange.push_back(std::make_pair(iIndex, bAdd));
		}

		SyncValue		stValue;
		std::unordered_map<uint32_t, SyncRepeated> mRepeated;
	};

}