#include "RedisBuffer.h"
#include <sstream>
#include <TData/KeyFieldMgr.h>

namespace Data
{

	void RedisBuffer::FieldBuffer::ValueChange(uint32_t iKeyField, const std::string & strValue)
	{
		mapField[iKeyField] = strValue;
	}

	void RedisBuffer::KeyBuffer::ValueChange(uint32_t iKeyField, uint64_t iFirstKey, uint64_t iSecondKey, const std::string & strValue)
	{
		auto& itFirst = mapBuffer[iFirstKey];
		auto& itSecond = itFirst[iSecondKey];
		itSecond.ValueChange(iKeyField, strValue);
	}


	RedisBuffer::RedisBuffer()
	{
	}


	RedisBuffer::~RedisBuffer()
	{
	}

	void RedisBuffer::AddValueChange(uint64_t i64Key, uint64_t iIndex, uint32_t iKeyField, const std::string & strValue)
	{
		m_mBuffer[iKeyField & (~0xFF)].ValueChange(iKeyField, i64Key, iIndex, strValue);
	}

	void RedisBuffer::Batch(std::function<void(const std::string&)> RedisCmd)
	{
		for (auto& itKey : m_mBuffer)
		{
			const char* sKey = KeyFieldMgr::FindKey(itKey.first);
			CheckKeyFiled(sKey);
			for (auto& itKeyFirst : itKey.second.mapBuffer)
			{
				for (auto& itKeySecond : itKeyFirst.second)
				{
					std::stringstream ssSave;
					ssSave << "HMSET " << sKey;
					std::stringstream ssPub;
					ssPub << "publish " << itKey.first << " S|" << itKeyFirst.first << "|" << itKeySecond.first << "|";
					if (itKeyFirst.first != 0)
					{
						ssSave << ":" << itKeyFirst.first;
						if (itKeySecond.first != 0)
							ssSave << ":" << itKeySecond.first;
					}
					ssSave << " ";
					for (auto& itKeyField : itKeySecond.second.mapField)
					{
						const char* sField = KeyFieldMgr::FindField(itKeyField.first);
						CheckKeyFiled(sField);
						ssSave << sField << " " << itKeyField.second << " ";
						ssPub << itKeyField.first << ":" << itKeyField.second << "|";
					}
					RedisCmd(ssSave.str());
					RedisCmd(ssPub.str());
				}
			}
		}
		Clear();
	}

	void RedisBuffer::Clear()
	{
		m_mBuffer.clear();
	}

}