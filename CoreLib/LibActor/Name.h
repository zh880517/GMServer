#pragma once

#include <LibActor/NamePool.h>

namespace Detail
{
	class Name
	{
	public:

		/**
		Default constructor.
		Constructs a null string with no value.
		*/
		Name() : mValue(0)
		{
		}

		/**
		Explicit constructor.
		*/
		explicit Name(const char *const str) : mValue(0)
		{
			if (str)
			{
				mValue = NamePool::Get(str);
			}
		}

		/**
		Returns true if the string has no value.
		*/
		bool IsNull() const
		{
			return (mValue == 0);
		}

		/**
		Gets the value of the string.
		\note Returns a null pointer if the string is null.
		*/
		const char *GetValue() const
		{
			return mValue;
		}

		/**
		Equality operator.
		*/
		bool operator == (const Name &other) const
		{
			// Pooled strings are unique so we can compare their addresses.
			// This works for null strings too, whose value pointers are zero.
			return (mValue == other.mValue);
		}

		/**
		Inequality operator.
		*/
		bool operator != (const Name &other) const
		{
			return !operator==(other);
		}

		/**
		Less-than operator, mainly for sorted containers.
		Defines an arbitrary, non-alphabetical partial ordering.
		*/
		bool operator < (const Name &other) const
		{
			// Arbitrary less-than based on cheap pointer comparison.
			// This works for null strings too, whose value pointers are zero.
			return (mValue < other.mValue);
		}

	private:

		const char *mValue;
	};
}