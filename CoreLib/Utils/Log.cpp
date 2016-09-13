#include "Log.h"
#include <iostream>


Log::Log()
	:m_pThread(nullptr)
{
	m_bStop.store(false);
	m_Handle = [](LogData* pLogData)
	{
		std::cout << pLogData->szFile << ":" << pLogData->iFileLine << " " << pLogData->ssLog.str() <<std::endl;
	};
}


Log::~Log()
{
	Stop();
	m_Mutex.lock();
	for (auto it :m_listLog)
	{
		delete it;
	}
	m_Mutex.unlock();
}

void Log::AddLog(LogData * pLogData)
{
	Instance().PushLog(pLogData);
}

void Log::SetLogHandle(std::function<void(LogData*pLogData)>& handele)
{
	m_Handle = handele;
}

void Log::Init()
{
	m_pThread = new std::thread([this]()
	{
		while (!m_bStop.load())
		{
			std::list<LogData *> listLog;
			m_Mutex.lock();
			std::swap(listLog, m_listLog);
			m_Mutex.unlock();
			for (auto it : listLog)
			{
				m_Handle(it);
				delete it;
			}
		}
	});
}

Log & Log::Instance()
{
	static Log s_Log;
	return s_Log;
}

void Log::PushLog(LogData * pLogData)
{
	m_Mutex.lock();
	m_listLog.push_back(pLogData);
	m_Mutex.unlock();
}

void Log::Stop()
{
	if (m_pThread != nullptr)
	{
		m_bStop.store(true);
		if (m_pThread->joinable())
		{
			m_pThread->join();
		}
		delete m_pThread;
		m_pThread = nullptr;
	}
}
