#pragma once

#include <map>
#include <unordered_map>
class MapUtils
{
public:
	template<typename T, typename K>
	static T* FindMapPtr(std::map<K, T>& mMap, const K& key)
	{
		auto it = mMap.find(key);
		if (mMap.end() != it)
		{
			return &(it->second);
		}
		return nullptr;
	}

	template<typename T, typename K>
	static T* FindMapPtr(std::unordered_map<K, T>& mMap, const K& key)
	{
		auto it = mMap.find(key);
		if (mMap.end() != it)
		{
			return &(it->second);
		}
		return nullptr;
	}


	template<typename T, typename K>
	static const T* FindMapPtr(const std::map<K, T>& mMap, const K& key)
	{
		auto it = mMap.find(key);
		if (mMap.cend() != it)
		{
			return &(it->second);
		}
		return nullptr;
	}

	template<typename T, typename K>
	static T FindMap(std::map<K, T>& mMap, const K& key)
	{
		auto it = mMap.find(key);
		if (mMap.end() != it)
		{
			return it->second;
		}
		return T();
	}

	template<typename T, typename K>
	static T FindMap(const std::map<K, T>& mMap, const K& key)
	{
		auto it = mMap.find(key);
		if (mMap.cend() != it)
		{
			return it->second;
		}
		return T();
	}

	template<typename T, typename K>
	static T FindMap(std::unordered_map<K, T>& mMap, const K& key)
	{
		auto it = mMap.find(key);
		if (mMap.end() != it)
		{
			return it->second;
		}
		return T();
	}

	template<typename T, typename K>
	static T FindMap(const std::unordered_map<K, T>& mMap, const K& key)
	{
		auto it = mMap.find(key);
		if (mMap.cend() != it)
		{
			return it->second;
		}
		return T();
	}
};
