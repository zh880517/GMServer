#include "RedisSubMgr.h"
#include <sstream>
#include <chrono>

namespace Data
{

	RedisSubMgr::RedisSubMgr()
	{
	}


	RedisSubMgr::~RedisSubMgr()
	{
	}

	RedisSubMgr & RedisSubMgr::Instance()
	{
		static RedisSubMgr sIntance;
		return sIntance;
	}

	bool RedisSubMgr::Init(const std::string & strHost, uint32_t iPort, uint32_t iTimeOutMS)
	{
		if (!m_RedisWrap.Init(strHost, iPort, false, iTimeOutMS))
			return false;
		m_RedisWrap.SetThreadFunc([this]() {
			std::string strCmd = GetSubString();
			if (!strCmd.empty())
			{
				m_RedisWrap.AsyncCommand(strCmd);
			}
			std::list<RedisSyncDataPtr> listData;
			bool bEmpty = m_RedisWrap.ReadSyncData(listData);

			if (listData.size() > 0)
			{
				for (auto& it : m_mListen)
				{
					it.second->AddSyncData(listData);
				}
			}
			if (bEmpty)
			{
				using namespace std::chrono_literals;
				std::this_thread::sleep_for(50ms);
			}
		});

		return true;
	}

	void RedisSubMgr::AddListen(uint32_t iActorID, RedisListen * pListen)
	{
		std::unique_lock<std::mutex> lock(m_MutexListenMap);
		m_mListen[iActorID] = pListen;
	}

	void RedisSubMgr::RemoveListen(uint32_t iActorID)
	{
		std::unique_lock<std::mutex> lock(m_MutexListenMap);
		auto it = m_mListen.find(iActorID);
		if (it != m_mListen.end())
		{
			m_mListen.erase(it);
		}
	}

	void RedisSubMgr::AddRediskey(const std::unordered_set<uint32_t>& vRedisKey)
	{
		std::unique_lock<std::mutex> lock(m_MutexRedisKey);
		for (auto it : vRedisKey)
		{
			if (m_AllRedisKey.find(it) == m_AllRedisKey.end())
			{
				m_AllRedisKey.insert(it);
				m_listRedisKey.push_back(it);
			}
		}
	}

	std::string RedisSubMgr::GetSubString()
	{
		std::list<uint32_t> listTemp;
		{
			std::unique_lock<std::mutex> lock(m_MutexRedisKey);
			std::swap(listTemp, m_listRedisKey);
		}
		if (listTemp.empty())
		{
			return std::string();
		}
		std::stringstream ssSub;
		ssSub << "SUBSCRIBE ";
		for (auto it : listTemp)
		{
			ssSub << it << " ";
		}
		return ssSub.str();
	}

}