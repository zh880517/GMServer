#ifndef _MFW_UTIL_MYSQL_
#define _MFW_UTIL_MYSQL_

#ifdef _MSC_VER
#include <windows.h>
#endif
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "StringUtils.h"
#include "mysql.h"

class CDBConf
{
public:
	std::string		sHost = "127.0.0.1";
	uint16_t		iPort = 3306;
	std::string		sDatabase;
	std::string		sUser = "root";
	std::string		sPassword = "123456";
	std::string		sCharset = "utf8";

	CDBConf() = default;

	CDBConf(const std::string& strHost, uint32_t nPort, 
		const std::string& strDB, const std::string& strUser, const std::string& strPassWord, 
		const std::string& strCharSet = "utf8") 
		: sHost(strHost)
		, iPort(nPort)
		, sDatabase(strDB)
		, sUser(strUser)
		, sPassword(strPassWord)
		, sCharset(strCharSet){}

	void Set(const std::map<std::string, std::string> &m);
};

class CMysqlResult
{
public:
	explicit CMysqlResult(MYSQL_RES *res);
	~CMysqlResult();

	bool Next();
	std::string operator[](uint32_t iIndex);
	std::string operator[](const std::string &sField);
	std::string Get(uint32_t iIndex);
	std::string Get(const std::string &sField);
	template <typename T> T Get(uint32_t iIndex) { return StringUtils::strto<T>(Get(iIndex)); }
	template <typename T> T Get(const std::string &sField) { return StringUtils::strto<T>(Get(sField)); }

	void Reset();
	uint64_t Count() { return m_iRowNum; }

private:
	MYSQL_RES *m_res;
	char **m_row;
	unsigned long *m_rowlength;
	std::map<std::string, std::uint32_t> m_mField;
	uint64_t m_iRowNum;

private:
	CMysqlResult(const CMysqlResult &) = delete;
	const CMysqlResult &operator=(const CMysqlResult &) = delete;
};

typedef std::shared_ptr<CMysqlResult> CMysqlResultPtr;

class CMysql
{
public:
	CMysql() : m_mysql(NULL) {}
	CMysql(const CDBConf &dbconf) : m_dbconf(dbconf), m_mysql(NULL) {}
	~CMysql();

	MYSQL *GetMysqlHandle() { return m_mysql; }

	void Init(const CDBConf &dbconf);
	void Connect();
	void Disconnect();
	std::string Escape(const std::string &s);
	void Execute(const std::string &sql);
	CMysqlResultPtr Query(const std::string &sql);

private:
	CDBConf		m_dbconf;
	MYSQL		*m_mysql;

private:
	CMysql(const CMysql &) = delete;
	const CMysql &operator=(const CMysql &) = delete;
};

#endif

