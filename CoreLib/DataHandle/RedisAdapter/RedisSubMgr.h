#pragma once
#include "RedisListen.h"
#include "RedisWrap.h"

namespace Data
{

	class RedisSubMgr
	{
	public:
		RedisSubMgr();
		~RedisSubMgr();

		static	RedisSubMgr& Instance();

		bool	Init(const std::string& strHost, uint32_t iPort, uint32_t iTimeOutMS = 150000);

		void	AddListen(uint32_t iActorID, RedisListen* pListen);

		void	RemoveListen(uint32_t iActorID);

		void	AddRediskey(const std::unordered_set<uint32_t>& vRedisKey);

		std::string GetSubString();


	private:
		RedisWrap							m_RedisWrap;
		std::mutex							m_MutexListenMap;
		std::map<uint32_t, RedisListen*>	m_mListen;

		std::mutex							m_MutexRedisKey;
		std::unordered_set<uint32_t>		m_AllRedisKey;
		std::list<uint32_t>					m_listRedisKey;
	};

}