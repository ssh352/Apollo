#pragma once
#include "stdafx.h"
#include "HxEventEngine.h"
#ifndef LOG_TO_FILE
#define LOG_TO_FILE 1  
#define LOG( FORMAT , ...) LOGGER::CTP_Log( " %s		     \n" FORMAT , CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") , __VA_ARGS__)
#define LOGDETAIL( FORMAT , ...) LOGGER::CTP_Log( "%s %s:%d		     \n" FORMAT , CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S") , __FUNCTION__ , __LINE__ , __VA_ARGS__)


#endif
#define MAX_LOG_LENGTH 10240
namespace CTPTOOL {
	 class BASELIB_API LOGGER
	{
	public:
		LOGGER();
		~LOGGER();
	public:
		static shared_ptr<LOGGER> loger;
		   HxEventEngine hxevent;
	private:
		CRITICAL_SECTION cm;
		FILE * logfile;
		int day;
		void Log(const char * format, va_list arg);
		char buf[MAX_LOG_LENGTH];
		char funtionname[MAX_LOG_LENGTH];
		int strategyid;
	public:
		static shared_ptr<LOGGER> GetInstance();
	public:
		void Log(const char * format, ...);
		void SetStrategyId(int id);
		 static void CTP_Log(const char * format, ...);
	};

	

}
