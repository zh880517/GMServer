#pragma once
#include <string>
#include <vector>
class StringHelper
{
public:
	static void get(const char* pStr, uint32_t& iValue)
	{
		iValue =(uint32_t)atoi(pStr);
	}

	static std::string ToString(uint32_t iValue)
	{
		char cBuff[100] = { 0 };
		snprintf(cBuff, sizeof(cBuff), "%u", iValue);
		return std::string(cBuff);
	}

	static void get(const char* pStr, int32_t& iValue)
	{
		iValue = atoi(pStr);
	}

	static std::string ToString(int32_t iValue)
	{
		char cBuff[100] = { 0 };
		snprintf(cBuff, sizeof(cBuff), "%d", iValue);
		return std::string(cBuff);
	}
	
	static void get(const char* pStr, int64_t& iValue)
	{
		iValue = _atoi64(pStr);
	}

	static std::string ToString(int64_t iValue)
	{
		char cBuff[100] = { 0 };
		snprintf(cBuff, sizeof(cBuff), "%lld", iValue);
		return std::string(cBuff);
	}

	static void get(const char* pStr, uint64_t& iValue)
	{
		iValue = (uint64_t)_atoi64(pStr);
	}

	static std::string ToString(uint64_t iValue)
	{
		char cBuff[100] = { 0 };
		snprintf(cBuff, sizeof(cBuff), "%llu", iValue);
		return std::string(cBuff);
	}

	static void get(const char* pStr, float& fValue)
	{
		fValue = (float)atof(pStr);
	}

	static std::string ToString(float fValue)
	{
		char cBuff[100] = { 0 };
		snprintf(cBuff, size_t(cBuff), "%0.4f", fValue);
		return std::string(cBuff);
	}

	static void get(const char* pStr, double& fValue)
	{
		fValue = atof(pStr);
	}

	static std::string ToString(double fValue)
	{
		char cBuff[100] = { 0 };
		snprintf(cBuff, size_t(cBuff), "%0.4lf", fValue);
		return std::string(cBuff);
	}

	static void get(const char* pStr, std::string& strValue)
	{
		strValue = pStr;
	}

	static std::string ToString(const std::string strValue)
	{
		return strValue;
	}

	static void get(const char* pStr, bool& bValue)
	{
		if (strcmp("true", pStr) == 0 || strcmp("1", pStr) == 0)
		{
			bValue = true;
		}
		else
		{
			bValue = false;
		}
	}

	static std::string ToString(bool bValue)
	{
		if (bValue)
		{
			return "true";
		}
		return "false";
	}

	static const char* SubString(const char* pSrc, const char* pSub)
	{
		for (uint32_t i = 0; ; ++i)
		{
			if (pSrc[i] == '\0')
				return nullptr;
			if (pSub[i] == '\0')
				return pSrc + i;
			if (pSrc[i] != pSub[i])
				return nullptr;
		}
		return nullptr;
	}

	static uint64_t GetKey64(const char* pSrc, const char* pSub)
	{
		return (uint64_t)_atoi64(SubString(pSrc, pSub));
	}

	static uint32_t ToU32(const char* pSrc)
	{
		return (uint32_t)atoi(pSrc);
	}

	static int32_t ToI32(const char* pSrc)
	{
		return atoi(pSrc);
	}

	static uint64_t ToU64(const char* pSrc)
	{
		return (uint64_t)_atoi64(pSrc);
	}

	static int64_t ToI64(const char* pSrc)
	{
		return _atoi64(pSrc);
	}

	static float ToFloat(const char* pStr)
	{
		return (float)atof(pStr);
	}

	static double ToDouble(const char* pStr)
	{
		return atof(pStr);
	}


	static bool ToBool(const char* pStr)
	{
		if (strcmp("true", pStr) == 0 || strcmp("1", pStr) == 0)
		{
			return true;
		}
		return false;
	}

	static int32_t FindFirstOf(const char* pStr, char seq)
	{
		int32_t iIndex = 0;
		while (pStr[iIndex] != '\n')
		{
			if (pStr[iIndex] == seq)
			{
				return iIndex;
			}
		}
		return -1;
	}

	static void Split(const std::string& str, std::vector<std::string>& vOut, char seq)
	{
		size_t iLast = 0;
		size_t iIndex = str.find_first_of(seq, iLast);
		while (iIndex != std::string::npos)
		{
			vOut.push_back(str.substr(iLast, iIndex - iLast));
			iLast = iIndex + 1;
			iIndex = str.find_first_of(seq, iLast);
		}
		if (str.size() - iLast > 0)
		{
			vOut.push_back(str.substr(iLast, str.size() - iLast));
		}
	}
};