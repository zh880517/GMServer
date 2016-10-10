#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <TData/StringValue.h>

namespace Data
{

	struct RedisSyncData
	{
		enum eSyncType
		{
			eNULL,
			eAdd,
			eDelete,
			eSet,
		};

		RedisSyncData(uint32_t iRedisKey) : RedisKey(iRedisKey), eType(eNULL) {}

		bool	Parse(const char* pStrMsg)
		{
			std::vector<std::string> vSplit;
			StringValue::Split(pStrMsg, vSplit, '|');
			if (vSplit.size() < 3)
				return false;
			Firstkey = std::stoull(vSplit[1]);
			SecondKey = std::stoull(vSplit[2]);
			switch (vSplit[0][0])
			{
			case 'A':
				eType = eAdd;
				break;
			case 'D':
				eType = eDelete;
				break;
			case 'S':
			{
				eType = eSet;
				if (vSplit.size() < 4)
					return false;
				for (size_t i = 3; i < vSplit.size(); ++i)
				{
					std::vector<std::string> vField;
					StringValue::Split(vSplit[i], vField, ':');
					if (vField.size() != 2)
						return false;
					uint32_t iKeyField = (uint32_t)std::stoi(vField[0]);
					if (vField.size() == 2)
					{
						mValue[iKeyField] = vField[1];
					}
				}
			}
			break;
			default:
				break;
			}
			return true;
		}
		static std::string GetString(const char* pStr, uint32_t& iOffSet, char seq = '|')
		{
			uint32_t iBegin = iOffSet;
			uint32_t iCorrect = 0;
			while (*(pStr + iOffSet) != '\0')
			{
				if (*(pStr + iOffSet) == seq)
				{
					++iOffSet;
					iCorrect = 1;
					break;
				}
				++iOffSet;
			}
			if (iOffSet == iBegin)
			{
				iCorrect = 0;
			}
			return std::string(pStr + iBegin, iOffSet - iBegin - iCorrect);
		}

		eSyncType	eType;
		uint32_t	RedisKey;
		uint64_t	Firstkey;
		uint64_t	SecondKey;
		std::map<uint32_t, std::string>	mValue;
	};

	using RedisSyncDataPtr = std::shared_ptr<RedisSyncData>;
}