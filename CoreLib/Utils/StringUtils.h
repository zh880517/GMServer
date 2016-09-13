#pragma once
#include <string>
#include <vector>
#include <map>
class StringUtils
{
public:

	static void get(const char* pStr, uint32_t& iValue)
	{
		iValue = (uint32_t)atoi(pStr);
	}

	static void get(const char* pStr, int32_t& iValue)
	{
		iValue = atoi(pStr);
	}

	static void get(const char* pStr, int64_t& iValue)
	{
		iValue = _atoi64(pStr);
	}

	static void get(const char* pStr, uint64_t& iValue)
	{
		iValue = (uint64_t)_atoi64(pStr);
	}
	
	static void get(const char* pStr, float& fValue)
	{
		fValue = (float)atof(pStr);
	}

	static void get(const char* pStr, double& fValue)
	{
		fValue = atof(pStr);
	}

	static void get(const char* pStr, std::string& strValue)
	{
		strValue = pStr;
	}

	static const char* subString(const char* pSrc, const char* pSub)
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

	template <typename T>
	static std::string tostr(const T &t)
	{
		ostringstream ss;
		ss << t;
		return ss.str();
	}

	template <typename T>
	static T strto(const std::string &s);

	template <typename T>
	static T strto(const std::string &s, const T &def)
	{
		if (s.empty()) return def;
		return strto<T>(s);
	}

	static void splitString(const std::string &str, const std::string &sep, bool bStrip, std::vector<std::string> &vResult);
	static std::vector<std::string> splitString(const std::string &str, const std::string &sep, bool bStrip = true);

	template <typename T> static void splitString(const std::string &str, const std::string &sep, bool bStrip, std::vector<T> &vResult);
	template <typename T> static std::vector<T> splitString(const std::string &str, const std::string &sep, bool bStrip = true);

	static std::string joinString(const std::vector<std::string> &v, const std::string &sep);
	template <typename T> static std::string joinString(const std::vector<T> &v, const std::string &sep);

	static void splitString2(const std::string &str, const std::string &sep1, const std::string &sep2, std::vector<std::pair<std::string, std::string> > &vResult);
	static void splitString2(const std::string &str, const std::string &sep1, const std::string &sep2, std::map<std::string, std::string> &mResult);
	static std::map<std::string, std::string> splitString2(const std::string &str, const std::string &sep1, const std::string &sep2);

	template <typename T1, typename T2>
	static void splitString2(const std::string &str, const std::string &sep1, const std::string &sep2, std::vector<std::pair<T1, T2> > &vResult);
	template <typename T1, typename T2>
	static void splitString2(const std::string &str, const std::string &sep1, const std::string &sep2, std::map<T1, T2> &mResult);
	template <typename T1, typename T2>
	static std::map<T1, T2> splitString2(const std::string &str, const std::string &sep1, const std::string &sep2);

	static std::string joinString2(const std::vector<std::pair<std::string, std::string> > &v, const std::string &sep1, const std::string &sep2);
	template <typename T1, typename T2>
	static std::string joinString2(const std::vector<std::pair<T1, T2> > &v, const std::string &sep1, const std::string &sep2);
	template <typename T1, typename T2>
	static std::string joinString2(const std::map<T1, T2> &v, const std::string &sep1, const std::string &sep2);
};


namespace impl
{
	template <typename T>
	struct lexical_castor
	{
		static T cast(const std::string &s)
		{
			std::istringstream ss(s);
			T t;
			ss >> t;
			return t;
		}
	};

	template<> struct lexical_castor <uint64_t>
	{
		static uint64_t cast(const std::string &s)
		{
			return std::strtoull(s.c_str(), NULL, 0);
		}
	};

	template<> struct lexical_castor <bool>
	{
		static bool cast(const std::string &s) { return lexical_castor<uint64_t>::cast(s) ? true : false; }
	};

	template<> struct lexical_castor <int8_t>
	{
		static int8_t cast(const std::string &s) { return static_cast<int8_t>(lexical_castor<uint64_t>::cast(s)); }
	};

	template<> struct lexical_castor <uint8_t>
	{
		static uint8_t cast(const std::string &s) { return static_cast<uint8_t>(lexical_castor<uint64_t>::cast(s)); }
	};

	template<> struct lexical_castor <int16_t>
	{
		static int16_t cast(const std::string &s) { return static_cast<int16_t>(lexical_castor<uint64_t>::cast(s)); }
	};

	template<> struct lexical_castor <uint16_t>
	{
		static uint16_t cast(const std::string &s) { return static_cast<uint16_t>(lexical_castor<uint64_t>::cast(s)); }
	};

	template<> struct lexical_castor <int32_t>
	{
		static int32_t cast(const std::string &s) { return static_cast<int32_t>(lexical_castor<uint64_t>::cast(s)); }
	};

	template<> struct lexical_castor <uint32_t>
	{
		static uint32_t cast(const std::string &s) { return static_cast<uint32_t>(lexical_castor<uint64_t>::cast(s)); }
	};

	template<> struct lexical_castor <int64_t>
	{
		static int64_t cast(const std::string &s) { return static_cast<int64_t>(lexical_castor<uint64_t>::cast(s)); }
	};

	template<> struct lexical_castor <float>
	{
		static float cast(const std::string &s)
		{
			return std::strtof(s.c_str(), NULL);
		}
	};

	template<> struct lexical_castor <double>
	{
		static double cast(const std::string &s)
		{
			return std::strtod(s.c_str(), NULL);
		}
	};

	template<> struct lexical_castor <std::string>
	{
		static const std::string &cast(const std::string &s)
		{
			return s;
		}
	};

}

template <typename T>
T StringUtils::strto(const std::string &s)
{
	return impl::lexical_castor<T>::cast(s);
}

template <typename T>
void StringUtils::splitString(const std::string &str, const std::string &sep, bool bStrip, std::vector<T> &vResult)
{
	std::vector<string> vStrResult;
	splitString(str, sep, bStrip, vStrResult);
	for (unsigned i = 0; i < vStrResult.size(); ++i)
	{
		T tmp = strto<T>(vStrResult[i]);
		vResult.push_back(tmp);
	}
}

template <typename T>
std::vector<T> StringUtils::splitString(const std::string &str, const std::string &sep, bool bStrip)
{
	std::vector<T> vResult;
	splitString<T>(str, sep, bStrip, vResult);
	return vResult;
}

template <typename T1, typename T2>
void StringUtils::splitString2(const std::string &str, const std::string &sep1, const std::string &sep2, std::vector< std::pair<T1, T2> > &vResult)
{
	std::vector<std::pair<string, string> > vStrResult;
	splitString2(str, sep1, sep2, vStrResult);
	for (unsigned i = 0; i < vStrResult.size(); ++i)
	{
		std::pair<std::string, std::string> &kv = vStrResult[i];
		vResult.push_back(std::make_pair(strto<T1>(kv.first), strto<T2>(kv.second)));
	}
}

template <typename T1, typename T2>
void StringUtils::splitString2(const std::string &str, const std::string &sep1, const std::string &sep2, std::map<T1, T2> &mResult)
{
	std::vector<pair<string, string> > vStrResult;
	splitString2(str, sep1, sep2, vStrResult);
	for (unsigned i = 0; i < vStrResult.size(); ++i)
	{
		std::pair<std::string, std::string> &kv = vStrResult[i];
		mResult[strto<T1>(kv.first)] = strto<T2>(kv.second);
	}
}

template <typename T1, typename T2>
std::map<T1, T2> StringUtils::splitString2(const std::string &str, const std::string &sep1, const std::string &sep2)
{
	std::map<T1, T2> mResult;
	splitString2<T1, T2>(str, sep1, sep2, mResult);
	return mResult;
}

template <typename T>
std::string StringUtils::joinString(const std::vector<T> &v, const std::string &sep)
{
	std::string sResult;
	for (unsigned i = 0; i < v.size(); ++i)
	{
		if (!sResult.empty())
		{
			sResult.append(sep);
		}
		sResult.append(tostr(v[i]));
	}
	return sResult;
}

template <typename T1, typename T2>
std::string StringUtils::joinString2(const std::vector<std::pair<T1, T2> > &v, const std::string &sep1, const std::string &sep2)
{
	std::string sResult;
	for (unsigned i = 0; i < v.size(); ++i)
	{
		if (!sResult.empty())
		{
			sResult.append(sep1);
		}

		const std::pair<T1, T2> &kv = v[i];
		sResult.append(tostr(kv.first));
		sResult.append(sep2);
		sResult.append(tostr(kv.second));
	}
	return sResult;
}

template <typename T1, typename T2>
std::string StringUtils::joinString2(const std::map<T1, T2> &v, const std::string &sep1, const std::string &sep2)
{
	std::string sResult;
	for (typename std::map<T1, T2>::const_iterator first = v.begin(), last = v.end(); first != last; ++first)
	{
		if (!sResult.empty())
		{
			sResult.append(sep1);
		}

		sResult.append(tostr(first->first));
		sResult.append(sep2);
		sResult.append(tostr(first->second));
	}
	return sResult;
}
