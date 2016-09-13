#pragma once
#include <stdio.h>
#include <assert.h>

#ifndef GW_ASSERT
#define GW_ASSERT(condition)  if (!(condition)) AssertUtils::Fail(__FILE__, __LINE__); else { }
#endif

#ifndef GW_ASSERT_MSG
#define GW_ASSERT_MSG(condition, msg)       if (!(condition)) AssertUtils::Fail(__FILE__, __LINE__, msg); else { }
#endif

class AssertUtils
{
public:
	static void	Fail(const char *const file, const unsigned int line, const char *const message = 0)
	{
		fprintf(stderr, "FAIL in %s (%d)", file, line);
		if (message)
		{
			fprintf(stderr, ": %s", message);
		}

		fprintf(stderr, "\n");
		assert(false);
	}
};