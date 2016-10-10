#pragma once
#include <TData/SyncValue.h>
#include <mutex>
#include <unordered_map>
#include <unordered_set>
#include <memory>
#include "RedisSyncData.h"

namespace Data
{

	class RedisListen
	{
	public:
		RedisListen();
		~RedisListen();

		void	Merge(uint64_t iKey, const std::unordered_set<uint32_t>& vRedisKey);

		void	AddSyncData(std::list<RedisSyncDataPtr>& listData);

		void	ExportSyncData(std::unordered_map<uint64_t, SyncStruct>& mSyncData);

	protected:
		std::mutex									m_Mutex;
		std::unordered_set<uint64_t>				m_vKey;
		std::unordered_set<uint32_t>				m_vRedisKey;
		std::list<RedisSyncDataPtr>					m_listSyncData;
		//std::unordered_map<uint64_t, SyncStruct>	m_mSync;
	};

}