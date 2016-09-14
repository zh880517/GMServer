#pragma once

#include "TypeDef.h"

enum eErrorCode
{
	eSuccess = 0,
};

struct ErrorCode
{
	int32_t			iErrorCode;//错误码
	int32_t			iLineNum;//产生错误码的行号
	const char*		sFileName;//产生错误码的文件
	const char*		sErrorCode;//错误码的字符串表示

	//只有错误码的构造函数
	ErrorCode(int32_t iCode):iErrorCode(iCode){}
	//完整的构造函数
	ErrorCode(int32_t iCode, const char* sName, const char* sFile, int32_t iline)
	:iErrorCode(iCode)
	,iLineNum(iline)
	,sFileName(sFile)
	,sErrorCode(sName)
	{}
	explicit operator bool() const 
	{	
		return (iErrorCode !=  eSuccess);
	}
};

const char * briefLogFileName(const char *name)
{
	const char *p = strrchr(name, '/');
	if (p != nullptr)
	{
		return p + 1;
	}
	return name;
}

#ifndef MAKE_ERROR
#define MAKE_ERROR(ERRORCODE)	ErrorCode(ERRORCODE, #ERRORCODE, __FILE__, __LINE__)
#endif
