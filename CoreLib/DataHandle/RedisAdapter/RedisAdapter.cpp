#include "RedisAdapter.h"
#include <sstream>
#include <TData/KeyFieldMgr.h>
#include "RedisWrap.h"
#include "RedisSubMgr.h"

namespace Data
{

	RedisAdapter::RedisAdapter()
		: m_pRedisWarp(nullptr)
		, m_iActorId(0)
	{
	}


	RedisAdapter::~RedisAdapter()
	{
		if (m_iActorId != 0)
		{
			RedisSubMgr::Instance().RemoveListen(m_iActorId);
		}
	}

	void RedisAdapter::Init(uint32_t iActorId)
	{
		m_iActorId = iActorId;
		if (m_iActorId > 0)
		{
			RedisSubMgr::Instance().AddListen(m_iActorId, &m_Listen);
		}
	}

	void RedisAdapter::SyncData()
	{
		std::unordered_map<uint64_t, SyncStruct> mSyncData;
		for (auto& it : mSyncData)
		{
			auto itFind = m_AllData.find(it.first);
			if (itFind != m_AllData.end())
			{
				for (auto& itStruct : itFind->second)
				{
					itStruct->Sync(it.second);
				}
			}
		}
	}

	void RedisAdapter::PubAndSet()
	{
		m_Buffer.Batch([this](const std::string& strCmd)
		{
			m_pRedisWarp->Execute(strCmd);
		});
	}

	void RedisAdapter::SetRedisWrap(RedisWrap * pWrap)
	{
		m_pRedisWarp = pWrap;
	}

	bool RedisAdapter::Load(uint64_t iKey, ValueMap & mValue)
	{
		std::map<uint32_t, std::list<IValue*>> mapLoad;
		std::unordered_set<uint32_t> vAddRedisKey;
		for (auto& it : mValue)
		{
			mapLoad[it.first & (~0xFF)].push_back(it.second);
		}
		for (auto& it : mapLoad)
		{
			const char* sKey = KeyFieldMgr::FindKey(it.first);
			CheckKeyFiled(sKey);

			vAddRedisKey.insert(it.first);

			std::stringstream ssQuery;
			ssQuery << "HMGET " << sKey << ":" << iKey << " ";
			for (auto& itValue : it.second)
			{
				const char* sField = KeyFieldMgr::FindField(itValue->KeyField());
				CheckKeyFiled(sField);
				ssQuery << sField << " ";
			}
			if (m_pRedisWarp == nullptr)
				throw std::runtime_error("None RedisWarp set!!!!");

			m_pRedisWarp->Load(ssQuery.str(), it.second);
		}
		m_Listen.Merge(iKey, vAddRedisKey);
		return true;
	}

	bool RedisAdapter::Load(uint64_t iKey, uint64_t iIndex, ValueMap & mValue)
	{
		std::map<uint32_t, std::list<IValue*>> mapLoad;

		std::unordered_set<uint32_t> vAddRedisKey;
		for (auto& it : mValue)
		{
			mapLoad[it.first & (~0xFF)].push_back(it.second);
		}
		for (auto& it : mapLoad)
		{
			const char* sKey = KeyFieldMgr::FindKey(it.first);
			if (sKey == nullptr)
				throw std::runtime_error("can't find redis key name!");

			vAddRedisKey.insert(it.first);

			std::stringstream ssQuery;
			ssQuery << "HMGET " << sKey << ":" << iKey << ":" << iIndex << " ";
			for (auto& itValue : it.second)
			{
				const char* sField = KeyFieldMgr::FindField(itValue->KeyField());
				if (sField == nullptr)
					throw std::runtime_error("can't find redis Field name!");
				ssQuery << sField << " ";
			}
			if (m_pRedisWarp == nullptr)
				throw std::runtime_error("None RedisWarp set!!!!");

			m_pRedisWarp->Load(ssQuery.str(), it.second);
		}
		m_Listen.Merge(iKey, vAddRedisKey);
		return true;
	}

	void RedisAdapter::Save(uint64_t iKey, ValueMap & mValue)
	{
		std::map<uint32_t, std::list<IValue*>> mapLoad;
		for (auto& it : mValue)
		{
			mapLoad[it.first & (~0xFF)].push_back(it.second);
		}
		for (auto& it : mapLoad)
		{
			const char* sKey = KeyFieldMgr::FindKey(it.first);
			if (sKey != nullptr)
				throw std::runtime_error("can't find redis key name!");

			std::stringstream ssSave;
			ssSave << "HMSET " << sKey << ":" << iKey << " ";
			for (auto& itValue : it.second)
			{
				const char* sField = KeyFieldMgr::FindField(itValue->KeyField());
				if (sField == nullptr)
					throw std::runtime_error("can't find redis Field name!");

				ssSave << sField << " " << itValue->ToString() << " ";
			}
			if (m_pRedisWarp == nullptr)
				throw std::runtime_error("None RedisWarp set!!!!");

			m_pRedisWarp->Execute(ssSave.str());
		}
	}

	void RedisAdapter::Save(uint64_t iKey, uint64_t iIndex, ValueMap & mValue)
	{
		std::map<uint32_t, std::list<IValue*>> mapLoad;
		for (auto& it : mValue)
		{
			mapLoad[it.first & (~0xFF)].push_back(it.second);
		}
		for (auto& it : mapLoad)
		{
			const char* sKey = KeyFieldMgr::FindKey(it.first);
			if (sKey == nullptr)
				throw std::runtime_error("can't find redis key name!");

			std::stringstream ssSave;
			ssSave << "HMSET " << sKey << ":" << iKey << ":" << iIndex << " ";
			for (auto& itValue : it.second)
			{
				const char* sField = KeyFieldMgr::FindField(itValue->KeyField());
				if (sField == nullptr)
					throw std::runtime_error("can't find redis Field name!");

				ssSave << sField << " " << itValue->ToString() << " ";
			}
			if (m_pRedisWarp == nullptr)
				throw std::runtime_error("None RedisWarp set!!!!");

			m_pRedisWarp->Execute(ssSave.str());
		}
	}

	void RedisAdapter::SetValue(uint64_t iKey, IValue * pValue)
	{
		m_Buffer.AddValueChange(iKey, 0, pValue->KeyField(), pValue->ToString());
	}

	void RedisAdapter::SetValue(uint64_t iKey, uint64_t iIndex, IValue * pValue)
	{
		m_Buffer.AddValueChange(iKey, 0, pValue->KeyField(), pValue->ToString());
	}

	void RedisAdapter::RegistStruct(IStruct* pIStruct)
	{
		m_AllData[pIStruct->GetKey()].insert(pIStruct);
	}

	void RedisAdapter::UnRegistStruct(IStruct* pIStruct)
	{
		m_AllData[pIStruct->GetKey()].erase(pIStruct);
	}

	std::list<uint64_t> RedisAdapter::GetAllKey(uint64_t i64Key, uint32_t iRediskey)
	{
		const char* sKey = KeyFieldMgr::FindKey(iRediskey);
		if (sKey == nullptr)
			throw std::runtime_error("can't find redis key name!");

		if (m_pRedisWarp == nullptr)
			throw std::runtime_error("None RedisWarp set!!!!");

		std::string strKey;
		{
			std::stringstream ss;
			ss << sKey << ":"<< i64Key << ":";
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
			iIndex = m_pRedisWarp->ScanKey(ssScan.str(), strKey, listOut);
			if (iIndex == 0)
				break;
		}

		return listOut;
	}

	void RedisAdapter::AddRepeated(uint64_t i64Key, uint64_t i64Index, uint32_t iRedisKey)
	{
		std::stringstream ssPub;
		ssPub << "publish " << iRedisKey << " A|" << i64Key << "|" << i64Index;
		m_pRedisWarp->Execute(ssPub.str());
	}

	void RedisAdapter::RemoveRepeated(uint64_t i64Key, uint64_t i64Index, uint32_t iRedisKey)
	{
		const char* sKey = KeyFieldMgr::FindKey(iRedisKey);
		CheckKeyFiled(sKey);
		std::stringstream ssDelete;
		ssDelete << "DEL " << sKey << ":" << i64Key << ":" << i64Index;
		m_pRedisWarp->Execute(ssDelete.str());

		std::stringstream ssPub;
		ssPub << "publish " << iRedisKey << " D|" << i64Key << "|" << i64Index;
		m_pRedisWarp->Execute(ssPub.str());
	}

}