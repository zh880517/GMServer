#pragma once

#include <mutex>
#include <Utils/AssertUtils.h>
namespace Detail
{
	template <class EntryType>
	class Directory
	{
	public:
		Directory();
		~Directory();

		uint32_t			Allocate(uint32_t index = -1);

		uint32_t			GetMaxIndex() { return m_iMaxIndex; }

		inline EntryType&	GetEntry(const uint32_t index);

		void				Lock() { m_Mutex.lock(); }
		void				UnLock() { m_Mutex.unlock(); }
	private:

		static const uint32_t ENTRIES_PER_PAGE = 256;  ///< Number of entries in each allocated page (power of two!).
		static const uint32_t MAX_PAGES = 32;         ///< Maximum number of allocated pages.

		struct Page
		{
			EntryType mEntries[ENTRIES_PER_PAGE];       ///< Array of entries making up this page.
		};

		Directory(const Directory &other);
		Directory &operator=(const Directory &other);

		mutable std::mutex		m_Mutex;                           ///< Ensures thread-safe access to the instance data.
		uint32_t				m_iNextIndex;                            ///< Auto-incremented index to use for next registered entity.
		Page*					m_Pages[MAX_PAGES];                        ///< Pointers to allocated pages.
		uint32_t				m_iMaxIndex;
		uint32_t				m_iReuseIndex;
	};


	template <class EntryType>
	Directory<EntryType>::Directory() :
		m_iNextIndex(0),
		m_iMaxIndex(0),
		m_iReuseIndex(0)
	{
		// Clear the page table.
		for (uint32_t page = 0; page < MAX_PAGES; ++page)
		{
			m_Pages[page] = nullptr;
		}
	}

	template <class EntryType>
	Directory<EntryType>::~Directory()
	{
		for (uint32_t page = 0; page < MAX_PAGES; ++page)
		{
			if (m_Pages[page])
			{
				// Destruct and free.
				delete m_Pages[page];
			}
		}
	}

	template <class EntryType>
	inline uint32_t Directory<EntryType>::Allocate(uint32_t index)
	{
		m_Mutex.lock();

		// Auto-allocate an index if none was specified.
		if (index == -1)
		{
			if (m_iNextIndex < MAX_PAGES * ENTRIES_PER_PAGE)
			{
				index = m_iNextIndex++;
			}
			else
			{
				//ID重用，遍历所有的Entry，如果有没被使用的，就重用它			
				for (uint32_t i = 0; i < MAX_PAGES * ENTRIES_PER_PAGE; ++i)
				{
					if (!GetEntry(i).IsUsed())
					{
						index = i;
						break;
					}
				}
				m_iReuseIndex = index + 1;
				if (m_iReuseIndex >= MAX_PAGES * ENTRIES_PER_PAGE)
				{
					m_iReuseIndex = 0;
				}
			}
		}

		// Allocate the page if it hasn't been allocated already.
		const uint32_t page(index / ENTRIES_PER_PAGE);
		if (m_Pages[page] == nullptr)
		{
			m_Pages[page] = new Page;
		}
		if (m_iNextIndex < index)
		{
			m_iMaxIndex = index;
		}
		m_Mutex.unlock();

		return index;
	}

	template <class EntryType>
	EntryType &Directory<EntryType>::GetEntry(const uint32_t index)
	{
		// Compute the page and offset.
		// TODO: Use a mask?
		const uint32_t page(index / ENTRIES_PER_PAGE);
		const uint32_t offset(index % ENTRIES_PER_PAGE);

		ASSERT_UTILS(page < MAX_PAGES);
		ASSERT_UTILS(offset < ENTRIES_PER_PAGE);
		ASSERT_UTILS(m_Pages[page]);

		return m_Pages[page]->mEntries[offset];
	}

}