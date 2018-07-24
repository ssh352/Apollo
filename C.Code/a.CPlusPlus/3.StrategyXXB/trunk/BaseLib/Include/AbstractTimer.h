#pragma once
#include <string>
#include "time.h"
using namespace std;
//抽象的时间类 用于获得该运行环境下的时间
namespace Abstract {

	class BASELIB_API AbstractTimer {

	public:
		virtual time_t GetTCurrentTime()=0;
		/**
		根据格式获得当前时间的

		**/
		virtual string GetStrCurrentTime(string format= "%Y-%m-%d %H:%M:%S")=0;
		virtual tm  GetTmCurrentTime()=0;

		static string GetStrTime(time_t t,string format="%Y-%m-%d %H:%M:%S");
		/*
		将str 装换为tm 
		*/
		static tm     GetTmFromStr(string time, string format = "%Y-%m-%d %H:%M:%S");
		/*
		将str 转换为time_t
		*/
		static time_t GetTimeFromStr(string time, string format = "%Y-%m-%d %H:%M:%S");
		tm   GetTmTime(time_t t);
	};
}
