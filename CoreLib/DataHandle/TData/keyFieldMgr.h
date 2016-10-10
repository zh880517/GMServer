#pragma once
#include <set>
#include <tuple>
#include <unordered_map>

namespace Data
{

	enum FieldType
	{
		FT_int32_t,
		FT_uint32_t,
		FT_int64_t,
		FT_uint64_t,
		FT_float,
		FT_double,
		FT_datetime,
		FT_bool,
		FT_string,
	};

	struct FiledNameType
	{
		const char* Name;
		FieldType	eType;
	};


	struct Datakey
	{
		const char* pName;
		std::unordered_map<uint32_t, FiledNameType > mField;

		const char*	GetField(uint32_t iField)const
		{
			auto it = mField.find(iField);
			if (it != mField.cend())
				return it->second.Name;
			return nullptr;
		}
	};

	class KeyFieldMgr
	{
	public:
		using KeyField = std::tuple<const char*, const char*>;
		static void	AddKey(const char* pKey, uint32_t iIndex)
		{
			uint32_t iKey = iIndex & ~(0xFF);
			Instance().mKey[iKey].pName = pKey;
		}

		static void	AddField(const char* pField, uint32_t iIndex, FieldType eType)
		{
			uint32_t iKey = iIndex & ~(0xFF);
			uint32_t iField = iIndex & 0xFF;
			Instance().mKey[iKey].mField[iField] = { pField , eType};
		}

		static void	AddPackageDB(const char* pDB, uint32_t iIndex)
		{
			uint32_t iKey = iIndex & ~(0xFF);
			Instance().mPackageDBName[std::string(pDB)] .insert(iIndex);
		}

		static void	AddRepeatedDB(const char* pDB, uint32_t iIndex)
		{
			uint32_t iKey = iIndex & ~(0xFF);
			Instance().mRepeatedDBName[std::string(pDB)] = iIndex;
		}

		static const Datakey* FindDataKey(uint32_t iKey)
		{
			auto itFind = Instance().mKey.find(iKey && (~0xFF));
			if (itFind != Instance().mKey.cend())
			{
				return &(itFind->second);
			}
			return nullptr;
		}

		static std::tuple<const char*, const char*> FindKeyField(uint32_t iKeyField)
		{
			const char* pKey = nullptr;
			const char* pField = nullptr;
			uint32_t iKey = iKeyField & ~(0xFF);
			uint32_t iField = iKeyField & 0xFF;

			const std::unordered_map<uint32_t, Datakey>& mapKey = Instance().mKey;

			auto itKey = mapKey.find(iKey);
			if (itKey != mapKey.cend())
			{
				pKey = itKey->second.pName;
				pField = itKey->second.GetField(iField);
			}
			return std::make_tuple(pKey, pField);
		}

		static const char*	FindKey(uint32_t iKey)
		{
			const std::unordered_map<uint32_t, Datakey>& mapKey = Instance().mKey;

			auto itKey = mapKey.find(iKey & ~(0xFF));
			if (itKey != mapKey.cend())
			{
				return itKey->second.pName;
			}
			return nullptr;
		}

		static const char* FindField(uint32_t iField)
		{
			uint32_t iKey = iField & ~(0xFF);
			uint32_t iRealField = iField & 0xFF;

			const std::unordered_map<uint32_t, Datakey>& mapKey = Instance().mKey;

			auto itKey = mapKey.find(iKey);
			if (itKey != mapKey.cend())
			{
				return itKey->second.GetField(iRealField);
			}
			return nullptr;
		}

		static const std::set<uint32_t>&	FindPackageDB(const std::string& strDBTableName)
		{
			const std::unordered_map < std:: string, std::set<uint32_t >> &mapDB = Instance().mPackageDBName;

			auto itKey = mapDB.find(strDBTableName);
			if (itKey != mapDB.cend())
			{
				return itKey->second;
			}
			static std::set<uint32_t> emptySet;
			return emptySet;
		}

		static const Datakey*	FindRepeatedDB(const std::string& strDBTableName)
		{
			const std::unordered_map < std::string, uint32_t > &mapDB = Instance().mRepeatedDBName;

			auto itKey = mapDB.find(strDBTableName);
			if (itKey != mapDB.cend())
			{
				auto itFind = Instance().mKey.find(itKey->second);
				if (itFind != Instance().mKey.cend())
				{
					return &(itFind->second);
				}
			}
			return nullptr;
		}

	protected:

		static KeyFieldMgr& Instance()
		{
			static KeyFieldMgr stInstance;
			return stInstance;
		}

	private:
		std::unordered_map<uint32_t, Datakey>	mKey;
		std::unordered_map<std::string , std::set<uint32_t>> mPackageDBName;
		std::unordered_map<std::string, uint32_t> mRepeatedDBName;
	};

#ifndef CheckKeyFiled
#define CheckKeyFiled(keyField) if(keyField == nullptr) throw std::runtime_error("can't find redis key field name!");
#endif // !CheckKeyFiled

}