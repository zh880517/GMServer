#include "MysqlUtils.h"
#include "errmsg.h"
#include <stdexcept>

void CDBConf::Set(const std::map<std::string, std::string> &m)
{
	std::map<std::string, std::string>::const_iterator it;
	if ((it = m.find("dbhost")) != m.end()) sHost = it->second;
	if ((it = m.find("dbport")) != m.end()) iPort = StringUtils::strto<uint16_t>(it->second);
	if ((it = m.find("dbname")) != m.end()) sDatabase = it->second;
	if ((it = m.find("dbuser")) != m.end()) sUser = it->second;
	if ((it = m.find("dbpass")) != m.end()) sPassword = it->second;
	if ((it = m.find("charset")) != m.end()) sCharset = it->second;
}

CMysqlResult::CMysqlResult(MYSQL_RES *res)
	: m_res(res), m_row(NULL),m_rowlength(NULL), m_iRowNum(0)
{
	if (m_res == NULL)
	{
		return;
	}

	uint32_t n = mysql_num_fields(m_res);
	MYSQL_FIELD *fields = mysql_fetch_fields(m_res);
	for (uint32_t i = 0; i < n; ++i)
	{
		m_mField[fields[i].name] = i;
	}
	m_iRowNum = mysql_num_rows(m_res);
}

CMysqlResult::~CMysqlResult()
{
	if (m_res)
	{
		mysql_free_result(m_res);
	}
}

bool CMysqlResult::Next()
{
	if (m_res)
	{
		m_row = mysql_fetch_row(m_res);
		if (m_row)
		{
			m_rowlength = mysql_fetch_lengths(m_res);
			return true;
		}
		else
		{
			m_rowlength = NULL;
			return false;
		}
	}
	return false;
}

std::string CMysqlResult::operator[](uint32_t iIndex)
{
	return Get(iIndex);
}

std::string CMysqlResult::operator[](const std::string &sField)
{
	return Get(sField);
}

std::string CMysqlResult::Get(uint32_t iIndex)
{
	if (m_res == NULL)
	{
		throw std::runtime_error("empty mysql result set");
	}
	if (iIndex >= m_mField.size())
	{
		throw std::runtime_error("exceed mysql result set field");
	}
	if (m_row == NULL)
	{
		throw std::runtime_error("invalid mysql result set row");
	}
	if (m_row[iIndex] == NULL || m_rowlength[iIndex] == 0)
	{
		return "";
	}
	return std::string(m_row[iIndex], m_rowlength[iIndex]);
}

std::string CMysqlResult::Get(const std::string &sField)
{
	std::map<std::string, uint32_t>::const_iterator it = m_mField.find(sField);
	if (it == m_mField.end())
	{
		throw std::runtime_error("mysql result set field doesn't exist: " + sField);
	}
	return (*this)[it->second];
}

void CMysqlResult::Reset()
{
	if (m_res)
	{
		mysql_data_seek(m_res, 0);
		m_row = NULL;
		m_rowlength = NULL;
	}
}

CMysql::~CMysql()
{
	Disconnect();
}

void CMysql::Init(const CDBConf &dbconf)
{
	Disconnect();
	m_dbconf = dbconf;
}

void CMysql::Connect()
{
	if (m_mysql != NULL)
	{
		return;
	}

	m_mysql = mysql_init(NULL);
	if (m_mysql == NULL)
	{
		throw std::runtime_error("mysql_init: " + std::string(mysql_error(m_mysql)));
	}

    if(!m_dbconf.sCharset.empty())
	{
    	int ret = mysql_options(m_mysql, MYSQL_SET_CHARSET_NAME, m_dbconf.sCharset.c_str());
		if (ret != 0)
		{
			std::string sError = mysql_error(m_mysql);
			mysql_close(m_mysql);
			m_mysql = NULL;
			throw std::runtime_error("mysql_options MYSQL_SET_CHARSET_NAME " + m_dbconf.sCharset + ": " + sError);
		}
	}

	char value = 1;
	mysql_options(m_mysql, MYSQL_OPT_RECONNECT, &value);

    if (mysql_real_connect(m_mysql, m_dbconf.sHost.c_str(), m_dbconf.sUser.c_str(), m_dbconf.sPassword.c_str(), m_dbconf.sDatabase.c_str(), m_dbconf.iPort, NULL, CLIENT_MULTI_STATEMENTS) == NULL)
    {
		std::string sError = mysql_error(m_mysql);
    	mysql_close(m_mysql);
		m_mysql = NULL;
    	throw std::runtime_error("mysql_real_connect: " + sError);
    }
}

void CMysql::Disconnect()
{
	if (m_mysql == NULL)
	{
		return;
	}
	mysql_close(m_mysql);
	m_mysql = NULL;
}

std::string CMysql::Escape(const std::string &s)
{
	Connect();

	std::string r;
	r.resize(s.size() * 2 + 1);
	mysql_real_escape_string(m_mysql, &r[0], s.c_str(), (unsigned long)s.size());
	r.resize(strlen(r.c_str()));
	return r;
}

void CMysql::Execute(const std::string &sql)
{
	Connect();

	int ret = mysql_ping(m_mysql);
	if (ret != 0)
	{
		Disconnect();
		Connect();
	}

	ret = mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.length());
	if (ret == 0)
	{
		return;
	}

	ret = mysql_errno(m_mysql);
	if (ret == CR_SERVER_GONE_ERROR || ret == CR_SERVER_LOST)
	{
		Disconnect();
		Connect();
		ret = mysql_real_query(m_mysql, sql.c_str(), (unsigned long)sql.length());
		if (ret == 0)
		{
			return;
		}
	}

	throw std::runtime_error("mysql_real_query: " + std::string(mysql_error(m_mysql)) + ", " + sql);
}

CMysqlResultPtr CMysql::Query(const std::string &sql)
{
	Execute(sql);
	MYSQL_RES *res = mysql_store_result(m_mysql);
	if (res == NULL)
	{
		throw std::runtime_error("mysql_store_result: " + std::string(mysql_error(m_mysql)) + ", " + sql);
	}

	return std::make_shared<CMysqlResult>(res);
}

