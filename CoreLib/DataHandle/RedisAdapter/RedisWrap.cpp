#include "RedisWrap.h"
#include <hiredis.h>
#include <iostream>
#include <TData/IBase.h>
#include <TData/StringValue.h>
#include <sstream>

namespace Data
{

	RedisWrap::RedisWrap()
		: m_pThread(nullptr)
		, m_pContext(nullptr)
		, m_iTimeOutMS(0)
		, m_iPort(0)
	{
	}


	RedisWrap::~RedisWrap()
	{
		Stop();
	}

	bool RedisWrap::Init(const std::string & strHost, uint32_t iPort, bool bBlock, uint32_t iTimeOutMS)
	{
		if (m_pContext != nullptr || m_pThread != nullptr)
			return false;

		m_strHost = strHost;
		m_iPort = iPort;
		m_bBlock = bBlock;
		m_iTimeOutMS = iTimeOutMS;
		return Connect();
	}

	bool RedisWrap::SetThreadFunc(const std::function<void()>& func)
	{
		if (m_bBlock || m_pThread != nullptr)
			return false;
		m_RunFlag.store(true);
		m_pThread = new std::thread([this, func]()
		{
			while (m_RunFlag.load())
			{
				func();
			}
		});

		return true;
	}

	void RedisWrap::Load(const std::string & strQuery, std::list<IValue*>& listValue)
	{
		redisReply * pResult = DoCmd(strQuery);
		if (pResult != nullptr && pResult->type == REDIS_REPLY_ARRAY)
		{
			for (uint32_t i = 0; i < pResult->elements; ++i)
			{
				IValue* pValue = listValue.front();
				if (pResult->element[i] != nullptr && pResult->element[i]->type == REDIS_REPLY_STRING)
				{
					pValue->Sync(pResult->element[i]->str);
				}
				listValue.pop_front();
			}
		}
		FreeRedisReply(pResult);
	}

	void RedisWrap::Load(const std::string& strQuery, std::list<std::string>& listValue)
	{
		redisReply * pResult = DoCmd(strQuery);
		if (pResult != nullptr && pResult->type == REDIS_REPLY_ARRAY)
		{
			for (uint32_t i = 0; i < pResult->elements; ++i)
			{
				if (pResult->element[i] != nullptr && pResult->element[i]->type == REDIS_REPLY_STRING)
				{
					listValue.push_back(pResult->element[i]->str);
				}
				else
				{
					listValue.push_back(std::string());
				}
			}
		}
		FreeRedisReply(pResult);
	}

	void RedisWrap::Execute(const std::string & strCmd)
	{
		redisReply *pReply = DoCmd(strCmd);
		FreeRedisReply(pReply);
	}

	uint32_t RedisWrap::ScanKey(const std::string & strScan, const std::string& strkey, std::list<uint64_t>& listKey)
	{
		redisReply * pResult = DoCmd(strScan);
		do
		{
			if (pResult == nullptr)break;
			if (pResult->type != REDIS_REPLY_ARRAY)break;
			if (pResult->elements < 2) break;
			uint32_t iIndex = (uint32_t)atoi(pResult->element[0]->str);
			redisReply *pTable = pResult->element[1];
			for (uint32_t i = 0; i < pTable->elements; ++i)
			{
				redisReply * pMatch = pTable->element[i];
				if (pMatch != nullptr && pMatch->type == REDIS_REPLY_STRING)
				{
					listKey.push_back(StringValue::GetKey64(pMatch->str, strkey.c_str()));
				}
			}
			return iIndex;
		} while (false);
		return 0;
	}

	void RedisWrap::ScanKey(uint64_t i64Key, const std::string strKeyName, std::list<uint64_t>& listKey)
	{
		std::stringstream ssScan;
		std::string strKey;
		{
			std::stringstream ss;
			ss << strKeyName << ":" << i64Key << ":";
			strKey = ss.str();
		}
		std::stringstream ssMatch;
		ssMatch << " MATCH " << strKey << "*";

		uint32_t iIndex = 0;
		std::list<uint64_t> listOut;
		while (true)
		{
			std::stringstream ssScan;
			ssScan << "SCAN " << iIndex << ssMatch.str();
			iIndex = ScanKey(ssScan.str(), strKey, listOut);
			if (iIndex == 0)
				break;
		}
	}


	bool RedisWrap::Connect()
	{
		if (m_bBlock)
		{
			m_pContext = redisConnect(m_strHost.c_str(), m_iPort);
		}
		else
		{
			m_pContext = redisConnectNonBlock(m_strHost.c_str(), m_iPort);
		}

		if (m_pContext == nullptr)
			return false;

		struct timeval timeout = { (long)m_iTimeOutMS / 1000, (long)(m_iTimeOutMS % 1000) * 1000 };
		redisSetTimeout(m_pContext, timeout);

		return true;
	}

	redisReply * RedisWrap::DoCmd(const std::string & strCmd)
	{
		std::cout << strCmd << std::endl;
		redisReply* pReply = (redisReply*)redisCommand(m_pContext, strCmd.c_str());
		if (pReply != nullptr && pReply->type == REDIS_REPLY_ERROR)
		{
			std::cout << "Redis Error :" << strCmd << std::endl;
			std::cout << "Error Infor : " << pReply->str << std::endl;
		}
		return pReply;
	}

	void RedisWrap::FreeRedisReply(redisReply * pReply)
	{
		if (pReply != nullptr)
		{
			freeReplyObject(pReply);
		}
	}

	void RedisWrap::Stop()
	{
		if (!m_bBlock && m_pThread != nullptr)
		{
			m_RunFlag.store(false);
			m_pThread->join();
		}

		if (m_pContext != nullptr)
		{
			redisFree(m_pContext);
			m_pContext = nullptr;
		}
		if (m_pThread != nullptr)
		{
			delete m_pThread;
			m_pThread = nullptr;
		}
	}

	void RedisWrap::AsyncCommand(const std::string & strCmd)
	{
		redisAppendCommand(m_pContext, strCmd.c_str());
		int iDone = 0;
		while (iDone == 0)
		{
			if (redisBufferWrite(m_pContext, &iDone) != REDIS_OK)
				break;
		}
	}

	bool RedisWrap::ReadSyncData(std::list<RedisSyncDataPtr>& listData)
	{
		redisBufferRead(m_pContext);
		while (true)
		{
			void *pData = nullptr;
			if (redisGetReply(m_pContext, &pData) != REDIS_OK) break;
			if (nullptr == pData) break;

			redisReply* pReply = (redisReply*)pData;

			if (pReply->type != REDIS_REPLY_ARRAY)
				continue;
			if (pReply->elements != 3)
				continue;
			if (strcmp("message", pReply->element[0]->str) != 0)
				continue;

			uint32_t iRedisKey = (uint32_t)atoi(pReply->element[1]->str);
			RedisSyncDataPtr pSyncData = std::make_shared<RedisSyncData>(iRedisKey);
			if (pSyncData && pSyncData->Parse(pReply->element[2]->str))
				listData.push_back(pSyncData);

			FreeRedisReply(pReply);
			if (listData.size() >= 10)
				return false;
		}
		return true;
	}

}