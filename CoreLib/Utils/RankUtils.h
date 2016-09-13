/*******************************************************************************
*		(C)年 xuancai Inc.,Ltd., All Rights Reserved
*		RankUtils.h
*		Created by dzhang 2016/07/29
*		Description: 排行榜辅助功能
*******************************************************************************/
#ifndef RANKUTILS_INCLUDE_DEF
#define RANKUTILS_INCLUDE_DEF
#include <map>
#include <vector>
#include <list>

template<typename KeyType, typename ValueType>
struct TRank
{
	TRank(uint32_t iMaxSize)
		:MaxSize(iMaxSize)
	{

	}
	void	Add(KeyType key, ValueType value)
	{
		vRank.push_back(std::make_pair(key, value));
		for (size_t i = vRank.size() - 1; i > 0; --i)
		{
			std::pair<KeyType, ValueType>& stBack = vRank[i];
			std::pair<KeyType, ValueType>& stFront = vRank[i - 1];
			if (vRank[i].second < vRank[i - 1].second)
				break;
			std::swap(vRank[i], vRank[i - 1]);
		}
		if (vRank.size() > MaxSize)
		{
			vRank.erase(vRank.begin() + vRank.size() - 1);
		}
	}
	uint32_t	MaxSize;
	std::vector< std::pair<KeyType, ValueType> > vRank;
};

template<typename KeyType, typename ValueType>
struct TRTRank
{
	using RankVec = std::list< std::pair<KeyType, ValueType> >;
	TRTRank(uint32_t iMaxSize = 10)
		:MaxSize(iMaxSize)
	{

	}

	void	Add(KeyType key, ValueType value)
	{
		RankVec::iterator itNew = vRank.end();
		if (vKey.find(key) != vKey.end())
		{
			for (RankVec::iterator it = vRank.begin(); it != vRank.end(); ++it)
			{
				if (it->first == key)
				{
					it->second += value;
					itNew = it;
					break;
				}
			}
		}
		else
		{
			vRank.push_back(std::make_pair(key, value));
			itNew = vRank.end();
			--itNew;
			vKey.insert(key);
		}
		if (vRank.size() == 1)
			return;

		while (itNew != vRank.begin())
		{
			RankVec::iterator itBefore = itNew;
			itBefore--;
			if (itNew->second <= itBefore->second)
				break;
			std::swap(itNew->first, itBefore->first);
			std::swap(itNew->second, itBefore->second);
			--itNew;
		}
		RankVec::iterator itBegin = vRank.begin();
		if (itNew->second > itBegin->second)
		{
			std::swap(itNew->first, itBegin->first);
			std::swap(itNew->second, itBegin->second);
		}
		if (vRank.size() > MaxSize)
		{
			vKey.erase(vRank.back().first);
			vRank.pop_back();
		}
	}

	bool	Empty() { return vRank.empty(); }

	bool	Remove(KeyType key)
	{
		if (vKey.find(key) != vKey.end())
		{
			for (RankVec::iterator it = vRank.begin(); it != vRank.end(); ++it)
			{
				if (it->first == key)
				{
					vRank.erase(it);
					break;
				}
			}
			vKey.erase(key);
			return true;
		}
		return false;
	}

	KeyType Front()
	{
		if (vRank.size() > 0)
		{
			return vRank.front().first;
		}
		return KeyType();
	}

	void	Clear()
	{
		vRank.clear();
		vKey.clear();
	}


protected:
	uint32_t	MaxSize;
	RankVec		vRank;
	std::set<KeyType> vKey;
};

#endif