#pragma once
#include <string>
#include <map>
#include <functional>

namespace Data
{

	class RedisBuffer
	{
		struct FieldBuffer
		{
			void	ValueChange(uint32_t iKeyField, const std::string&  strValue);

			std::map<uint32_t, std::string>	mapField;
		};

		struct KeyBuffer
		{
			void	ValueChange(uint32_t iKeyField, uint64_t iFirstKey, uint64_t iSecondKey, const std::string& strValue);

			std::map<uint64_t, std::map<uint64_t, FieldBuffer> > mapBuffer;
		};
	public:
		RedisBuffer();
		~RedisBuffer();

		void	AddValueChange(uint64_t i64Key, uint64_t iIndex, uint32_t iKeyField, const std::string& strValue);

		void	Batch(std::function<void(const std::string&)> RedisCmd);

		void	Clear();

	private:
		std::map<uint32_t, KeyBuffer>	m_mBuffer;
	};

}