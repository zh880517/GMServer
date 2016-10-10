#include "RedisListen.h"
#include "RedisSubMgr.h"

namespace Data
{

	RedisListen::RedisListen()
	{
	}


	RedisListen::~RedisListen()
	{
	}

	void RedisListen::Merge(uint64_t iKey, const std::unordered_set<uint32_t>& vRedisKey)
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_vKey.insert(iKey);
			m_vRedisKey.insert(vRedisKey.begin(), vRedisKey.end());
		}
		RedisSubMgr::Instance().AddRediskey(vRedisKey);
	}

	void RedisListen::AddSyncData(std::list<RedisSyncDataPtr>& listData)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		for (auto& it : listData)
		{
			if (m_vRedisKey.find(it->RedisKey) == m_vRedisKey.end() || m_vKey.find(it->Firstkey) == m_vKey.end())
				continue;
			m_listSyncData.push_back(it);
		}
	}

	void RedisListen::ExportSyncData(std::unordered_map<uint64_t, SyncStruct>& mSyncData)
	{
		std::unique_lock<std::mutex> lock(m_Mutex);
		for (auto& it : m_listSyncData)
		{
			switch (it->eType)
			{
			case RedisSyncData::eAdd:
				mSyncData[it->Firstkey].RepeatedModify(it->SecondKey, it->RedisKey, true);
				break;
			case RedisSyncData::eDelete:
				mSyncData[it->Firstkey].RepeatedModify(it->SecondKey, it->RedisKey, true);
				break;
			case RedisSyncData::eSet:
			{
				SyncStruct& stData = mSyncData[it->Firstkey];
				for (auto& itValue : it->mValue)
				{
					stData.AddValue(it->SecondKey, it->RedisKey, itValue.second);
				}
			}
			break;
			default:
				break;
			}
		}
		m_listSyncData.clear();
	}

}