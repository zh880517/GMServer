#pragma once
#include <list>
#include <map>
#include <mutex>

namespace Detail
{
	/*
		�̰߳�ȫ�����ֳأ�ÿ���ַ���ֻ��һ�ݣ�ͨ��Get()�ӿڽ���ͨ���ַ���ת�����Ѿ������const char*
		��ҪӦ����Name��ÿ�ζԱ�Name��ʱ��ֻ�öԱȱ����cosnt char*��ֵ�Ϳ���
	*/
	class NamePool
	{
		class Bucket
		{
		public:
			const char* Lookup(const char* const str)
			{
				std::list<std::string>::iterator it = m_StringList.begin();
				while (it != m_StringList.end())
				{
					if (strcmp(it->c_str(), str) == 0)
						return it->c_str();
					++it;
				}
				m_StringList.push_back(std::string(str));
				return m_StringList.back().c_str();
			}
		private:
			std::list<std::string> m_StringList;
		};

	public:

		inline static uint32_t		Hash(const char *const str)
		{
			const char *ch(str);
			uint32_t hash(5381);

			while (*ch != '\0')
			{
				hash = ((hash << 5) + hash) + *ch;
				++ch;
			}

			return hash;
		}
		inline static const char*	Get(const char *const str)
		{
			/*
				��C++11�����µ�д�����̰߳�ȫ�ģ�����Ҫ��static����ĳ�ʼ������
			*/
			static NamePool stInstance;
			return stInstance.Lookup(str);
		}
	protected:
		const char*					Lookup(const char *const str)
		{
			if (str == 0) return 0;
			const uint32_t iIndex(Hash(str));
			std::unique_lock<std::mutex> _tempLock(m_Mutex);
			Bucket& refBucket = m_mapBucket[iIndex];
			return refBucket.Lookup(str);
		}
		NamePool(){}
	private:
		std::mutex						m_Mutex;
		std::map<int32_t, Bucket>		m_mapBucket;
	};
}
