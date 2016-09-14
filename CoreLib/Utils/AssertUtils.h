#pragma once
#include <stdio.h>
#include <assert.h>

#ifndef ASSERT_UTILS
#define ASSERT_UTILS(condition)  if (!(condition)) AssertUtils::Fail(__FILE__, __LINE__); else { }
#endif

#ifndef ASSERT_UTILS_MSG
#define ASSERT_UTILS_MSG(condition, msg)       if (!(condition)) AssertUtils::Fail(__FILE__, __LINE__, msg); else { }
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