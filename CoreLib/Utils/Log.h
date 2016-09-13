#pragma once
#include <string>
#include <sstream>
#include <functional>
#include <mutex>
#include <thread>
#include <list>
#include <atomic>

#ifndef _MSC_VER
#define  SEQ '/'
#else
#define  SEQ '\\'
#endif


enum LogType
{
	eLog_Debug = 1,
	eLog_Infor = 2,
	eLog_Error = 4,
};

struct LogData
{
	LogData(uint32_t iType, const char* pFile, uint32_t iLine) 
		: iLogType(iType), szFile(pFile), iFileLine(iLine)
	{

	}
	uint32_t			iLogType;
	std::stringstream	ssLog;
	const char*			szFile;
	uint32_t			iFileLine;
};

class Log
{
public:
	Log();
	~Log();

	static	void		AddLog(LogData* pLogData);

	void				SetLogHandle(std::function<void(LogData* pLogData)>& handele);

	void				Init();

	static Log&			Instance();

	void				PushLog(LogData* pLogData);

	void				Stop();

	static const char * BriefLogFileName(const char *name)
	{
		const char *p = strrchr(name, SEQ);
		if (p != nullptr)
		{
			return p + 1;
		}
		return name;
	}
private:
	std::thread*							m_pThread;
	std::mutex								m_Mutex;
	std::atomic_bool						m_bStop;
	std::list<LogData*>						m_listLog;
	std::function<void(LogData* pLogData)>	m_Handle;
};


#define  LOG_EBUG(data)			\
{	\
	LogData* pLogData = new LogData(eLog_Debug, Log::BriefLogFileName(__FILE__), __LINE__)	;\
	pLogData->ssLog << data;		\
	Log::AddLog(pLogData);			\
}

#define  LOG_INFOR(data)			\
{	\
	LogData* pLogData = new LogData(eLog_Infor,  Log::BriefLogFileName(__FILE__), __LINE__)	;\
	pLogData->ssLog << data;		\
	Log::AddLog(pLogData);			\
}

#define  LOG_ERROR(data)			\
{	\
	LogData* pLogData = new LogData(eLog_Error,  Log::BriefLogFileName(__FILE__), __LINE__)	;\
	pLogData->ssLog << data;		\
	Log::AddLog(pLogData);			\
}

