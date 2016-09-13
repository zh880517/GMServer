#pragma once

#include <random>

class BaseRandom
{
public:
	BaseRandom(int32_t iBegin = 0, int32_t iEnd = 10000)
		: m_gen(m_rd()), m_dis(iBegin, iEnd)
	{
	}
	~BaseRandom() = default;

	int32_t	Random()
	{
		return m_dis(m_gen);
	}

private:
	std::random_device	m_rd;
	std::mt19937		m_gen;
	std::uniform_int_distribution<int32_t>	m_dis;
};
