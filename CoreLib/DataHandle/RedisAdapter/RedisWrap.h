#pragma once
#include <string>
#include <list>
#include <thread>
#include <atomic>
#include "RedisSyncData.h"

struct redisContext;
struct redisReply;
namespace Data
{
	class IValue;

	struct RedisConfig
	{
		std::string strHost = "127.0.0.1";
		uint32_t	iPort = 6379;
		bool		bBlock = true;
		uint32_t	iTimeOutMS = 150000;
	};

	class RedisWrap
	{
	public:
		RedisWrap();
		~RedisWrap();

		bool	Init(const std::string& strHost, uint32_t iPort, bool bBlock, uint32_t iTimeOutMS = 150000);

		bool	SetThreadFunc(const std::function<void()>& func);

		void	Load(const std::string& strQuery, std::list<IValue*>& listValue);

		void	Load(const std::string& strQuery, std::list<std::string>& listValue);

		void	Execute(const std::string& strCmd);

		uint32_t	ScanKey(const std::string& strScan, const std::string& strkey, std::list<uint64_t>& listKey);

		void	ScanKey(uint64_t i64Key, const std::string strKeyName, std::list<uint64_t>& listKey);

		void	Stop();

		void	AsyncCommand(const std::string& strCmd);

		bool	ReadSyncData(std::list<RedisSyncDataPtr>& listData);

	protected:

		bool	Connect();

		redisReply*	DoCmd(const std::string& strCmd);

		void	FreeRedisReply(redisReply* pReply);

	private:

		bool					m_bBlock;
		std::string				m_strHost;
		uint32_t				m_iPort;
		uint32_t				m_iTimeOutMS;
		redisContext*			m_pContext;

		std::thread*			m_pThread;

		std::atomic_bool		m_RunFlag;
	};

}