#pragma once

#include <RedisAdapter/RedisWrap.h>
#include <MySqlAdapter/util_mysql.h>
#include <TData/keyFieldMgr.h>

namespace Data
{
	class DataTransfer
	{
	public:
		DataTransfer(const RedisConfig& stRedisCfg, const CDBConf& stDBcfg);
		~DataTransfer();

		bool	LoadPackageToRedis(const std::string& strDBTable, uint64_t i64Key);
		bool	LoadRepeatedToRedis(const std::string& strDBTable, uint64_t i64Key);

		bool	SavePackageToDB(const std::string& strDBTable, uint64_t i64Key);
		bool	SaveRepeatedToDB(const std::string& strDBTable, uint64_t i64Key);

	protected:
		std::string	TranToRedisStr(const std::string& strValue, FieldType eType);

		std::string	TranToDBStr(const std::string& strValue, FieldType eType);

	private:
		CMysql		m_MySql;
		RedisWrap	m_Redis;
	};

}