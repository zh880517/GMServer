#include "DataTransfer.h"
#include <sstream>
#include <iostream>
#include <ctime>

namespace Data
{
	DataTransfer::DataTransfer(const RedisConfig & stRedisCfg, const CDBConf & stDBcfg)
		:m_MySql(stDBcfg)
	{
		m_Redis.Init(stRedisCfg.strHost, stRedisCfg.iPort, true, stRedisCfg.iTimeOutMS);
		m_MySql.Connect();
	}

	DataTransfer::~DataTransfer()
	{
	}

	bool DataTransfer::LoadPackageToRedis(const std::string & strDBTable, uint64_t i64Key)
	{
		try
		{
			const std::set<uint32_t>& setKey = KeyFieldMgr::FindPackageDB(strDBTable);
			if (setKey.empty())
				return false;
			std::list<const Datakey*> vKey;
			for (auto it : setKey)
			{
				const Datakey* pKey = KeyFieldMgr::FindDataKey(it);
				if (pKey != nullptr)
					vKey.push_back(pKey);
			}
			std::stringstream ssQuery;
			ssQuery << "SELECT ";
			for (auto* it : vKey)
			{
				uint32_t iIndex = 1;
				for (auto& itFiled : it->mField)
				{
					ssQuery << itFiled.second.Name;
					if (iIndex++ != it->mField.size())
					{
						ssQuery << ",";
					}
					ssQuery << " ";
				}
			}
			ssQuery << "FROM " << strDBTable << " where Key=" << i64Key;

			CMysqlResultPtr pResult = m_MySql.Query(ssQuery.str());
			if (!pResult || !pResult->Next())
				return false;
			for (auto it:vKey)
			{
				std::stringstream ssRedis;
				ssRedis << "HMSET " << it->pName << ":" << i64Key;
				for (auto& itField:it->mField)
				{
					ssRedis << " " << itField.second.Name << " " << TranToRedisStr(pResult->Get(itField.second.Name), itField.second.eType);
				}
				m_Redis.Execute(ssRedis.str());
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
		return true;
	}
	bool DataTransfer::LoadRepeatedToRedis(const std::string & strDBTable, uint64_t i64Key)
	{
		try
		{
			const Datakey* pKey = KeyFieldMgr::FindRepeatedDB(strDBTable);
			if (pKey == nullptr)
				return false;
			std::stringstream ssQuery;
			ssQuery << "SELECT Index, ";
			uint32_t iIndex = 1;
			for (auto& itFiled : pKey->mField)
			{
				ssQuery << itFiled.second.Name;
				if (iIndex++ != pKey->mField.size())
				{
					ssQuery << ",";
				}
				ssQuery << " ";
			}
			ssQuery <<"FROM "<< strDBTable << " where Key=" << i64Key;
			CMysqlResultPtr pResult = m_MySql.Query(ssQuery.str());
			if (!pResult )
				return false;
			//移除redis中原有的repeated数据
			{
				std::stringstream ssRedis;
				ssRedis << "KEYS " << pKey->pName << ":" << i64Key << ":*";
				std::list<std::string> list;
				m_Redis.Load(ssRedis.str(), list);
				for (auto& it:list)
				{
					std::stringstream ss;
					ss << "DEL " << it;
					m_Redis.Execute(ss.str());
				}
			}
			while (pResult->Next())
			{
				std::stringstream ssRedis;
				ssRedis << "HMSET " << pKey->pName << ":" << i64Key << ":" << pResult->Get("Index");
				for (auto& itField:pKey->mField)
				{
					ssRedis << " " << itField.second.Name << " " << TranToRedisStr(pResult->Get(itField.second.Name), itField.second.eType);
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
		}
		return true;
	}
	bool DataTransfer::SavePackageToDB(const std::string & strDBTable, uint64_t i64Key)
	{
		try
		{
			const std::set<uint32_t>& setKey = KeyFieldMgr::FindPackageDB(strDBTable);
			if (setKey.empty())
				return false;
			std::list<const Datakey*> vKey;
			for (auto it : setKey)
			{
				const Datakey* pKey = KeyFieldMgr::FindDataKey(it);
				if (pKey != nullptr)
					vKey.push_back(pKey);
			}
			bool bExit = true;
			{
				std::stringstream ssQuery;
				ssQuery << "Select * FROM " << strDBTable << " WHERE Key = " << i64Key;
				CMysqlResultPtr pResult = m_MySql.Query(ssQuery.str());
				if (!pResult)
					return false;
				if (!pResult->Next())
					bExit = false;
			}
			std::map<std::string, std::string> mKeyValue;
			for (auto it:vKey)
			{
				std::stringstream ssRedis;
				ssRedis << "HMGET " << it->pName << ":" << i64Key;
				for (auto& itFiled:it->mField)
				{
					ssRedis << " " << itFiled.second.Name;
				}
				std::list<std::string> listResult;
				m_Redis.Load(ssRedis.str(), listResult);
				if (listResult.size() != it->mField.size())
				{
					std::cout << "Redis Load fail!!!" << std::endl;
					return false;
				}
				uint32_t iIndex = 1;
				for (auto& itFiled : it->mField)
				{
					if (!listResult.front().empty())
					{
						mKeyValue[itFiled.second.Name] = TranToDBStr(listResult.front(), itFiled.second.eType);
					}
					listResult.pop_front();
				}
			}
			if (bExit)
			{
				std::stringstream ssQuery;
				ssQuery << "UPDATE " << strDBTable << " SET ";
				uint32_t iIndex = 1;
				for (auto& it:mKeyValue)
				{
					ssQuery << it.first << "=" << it.second;
					if (iIndex++ != mKeyValue.size())
					{
						ssQuery << ",";
					}
					else
					{
						ssQuery << " ";
					}
				}
				ssQuery << "WHERE Key=" << i64Key;
				m_MySql.Execute(ssQuery.str());
			} 
			else
			{
				std::stringstream ssQuery;
				ssQuery << "INSERT INTO " << strDBTable << " (Key,";
				std::stringstream ssField;
				std::stringstream ssValue;
				uint32_t iIndex = 1;
				for (auto& it : mKeyValue)
				{
					ssField << it.first;
					ssValue << it.second;
					if (iIndex++ != mKeyValue.size())
					{
						ssField << ",";
						ssValue << ",";
					}
				}
				ssQuery << ssField.str() << ") VALUES(" << i64Key << "," << ssValue.str() << ")";
				m_Redis.Execute(ssQuery.str());
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
		return true;
	}
	bool DataTransfer::SaveRepeatedToDB(const std::string & strDBTable, uint64_t i64Key)
	{
		try
		{
			const Datakey* pKey = KeyFieldMgr::FindRepeatedDB(strDBTable);
			if (pKey == nullptr)
				return false;
			std::list<uint64_t> listKey;
			m_Redis.ScanKey(i64Key, pKey->pName, listKey);
			//暂时全部删除
			{
				std::stringstream ssDelete;
				ssDelete << "DELETE FROM " << strDBTable << " WHERE Key=" << i64Key;
				m_MySql.Execute(ssDelete.str());
			}

			std::string strInsert;
			{
				std::stringstream ssQuery;
				ssQuery << "INSERT INTO " << strDBTable << " ( Key, Index, ";
				uint32_t iIndex = 1;
				for (auto& it : pKey->mField)
				{
					ssQuery << it.second.Name;
					if (iIndex++ != pKey->mField.size())
						ssQuery << ",";
					ssQuery << " ";
				}
				ssQuery << ") VALUES (" << i64Key << ",";

				strInsert = ssQuery.str();
			}
			for (auto it:listKey)
			{
				std::stringstream ssRedis;
				ssRedis << "HMGET " << pKey->pName << ":" << i64Key << ":" << it;
				std::list<std::string> listValue;
				m_Redis.Load(ssRedis.str(), listValue);
				if (listValue.size() != pKey->mField.size())
				{
					std::cout << "Redis Load fail!!!" << std::endl;
					return false;
				}
				std::stringstream ssQuery;
				ssQuery << strInsert << it << ", ";
				uint32_t iIndex = 1;
				for (auto& it : pKey->mField)
				{
					if (!listValue.front().empty())
					{
						ssQuery << TranToDBStr(listValue.front(), it.second.eType);
						if (iIndex++ != pKey->mField.size())
							ssQuery << ",";
						ssQuery << " ";
					}
					listValue.pop_front();
				}
				if (iIndex > 1)
				{
					ssQuery << ")";
					m_MySql.Execute(ssQuery.str());
				}
			}
		}
		catch (const std::exception& e)
		{
			std::cout << e.what() << std::endl;
			return false;
		}
		return true;
	}
	std::string DataTransfer::TranToRedisStr(const std::string & strValue, FieldType eType)
	{
		switch (eType)
		{
		case FT_int32_t:
		case FT_uint32_t:
		case FT_int64_t:
		case FT_uint64_t:
		case FT_float:
		case FT_double:
		case FT_bool:
			return strValue;
			break;
		case FT_datetime:
		{
			struct tm stTime;
			sscanf_s(strValue.c_str(), "%d-%d-%d %d:%d:%d",
				&stTime.tm_year, &stTime.tm_mon, &stTime.tm_mday, &stTime.tm_hour, &stTime.tm_min, &stTime.tm_sec);
			stTime.tm_year -= 1900;
			stTime.tm_mon -= 1;
			std::time_t tTime = std::mktime(&stTime);
			return StringValue::ToString(tTime);
		}
		break;
		case FT_string:
			return StringValue::ToString(strValue);
		break;
		default:
			break;
		}
		return "";
	}

	std::string DataTransfer::TranToDBStr(const std::string& strValue, FieldType eType)
	{
		switch (eType)
		{
		case FT_int32_t:
		case FT_uint32_t:
		case FT_int64_t:
		case FT_uint64_t:
		case FT_float:
		case FT_double:
		case FT_bool:
			return strValue;
			break;
		case FT_datetime:
		{
			std::time_t tTime;
			StringValue::get(strValue, tTime);
			struct tm stTime;
			localtime_s(&stTime, &tTime);
			char cBUFF[100] = { 0 };
			snprintf(cBUFF, sizeof(cBUFF), "'%d-%d-%d %d:%d:%d'", 
				stTime.tm_year + 1900, stTime.tm_mon + 1, stTime.tm_mday, stTime.tm_hour, stTime.tm_min, stTime.tm_sec);
			return std::string(cBUFF);
		}
		break;
		case FT_string:
		{
			std::string str = strValue;
			str[0] = '\'';
			str[str.size() - 1] = '\'';
			return str;
		}
		break;
		default:
			break;
		}
		return "";
	}

}