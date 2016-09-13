#pragma once
#include <stdint.h>
#include <string>
class HashName
{
public:
	explicit HashName(const char* pStr, uint32_t iHash)
		:m_strValue(pStr), m_iHash(iHash)
	{
	}
	explicit HashName(const std::string& strValue, uint32_t iHash)
		:m_strValue(strValue), m_iHash(iHash)
	{
	}
	explicit HashName(const char* pStr) 
		:m_strValue(pStr)
	{
		m_iHash = Hash(pStr);
	}

	explicit HashName(const std::string& strValue)
		:m_strValue(strValue)
	{
		m_iHash = Hash(m_strValue);
	}

	explicit HashName()
		:m_iHash(0)
	{

	}

	bool operator == (const HashName &other) const
	{
		if (m_iHash != other.m_iHash)
			return false;
		if (m_strValue == other.m_strValue)
			return true;

		return m_strValue == other.m_strValue;
	}

	bool operator < (const HashName &other)const
	{
		if (m_iHash == other.m_iHash)
		{
			return m_strValue < other.m_strValue;
		}
		return m_iHash < other.m_iHash;
	}
	bool operator != (const HashName &other) const
	{
		return !(*this == other);
	}

	static uint32_t Hash(const char* pStr)
	{
		const char *ch(pStr);
		uint32_t hash(5381);

		while (*ch != '\0')
		{
			hash = ((hash << 5) + hash) + *ch;
			++ch;
		}

		return hash;
	}

	static uint32_t Hash(const std::string& strValue)
	{
		uint32_t hash(5381);
		for (auto& it: strValue)
		{
			hash = ((hash << 5) + hash) + it;
		}
		return hash;
	}
	const std::string& String()const { return m_strValue; }
private:
	std::string		m_strValue;
	uint32_t		m_iHash;
};