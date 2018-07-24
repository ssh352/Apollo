#pragma once
#include <string>
#include "time.h"
using namespace std;
//�����ʱ���� ���ڻ�ø����л����µ�ʱ��
namespace Abstract {

	class BASELIB_API AbstractTimer {

	public:
		virtual time_t GetTCurrentTime()=0;
		/**
		���ݸ�ʽ��õ�ǰʱ���

		**/
		virtual string GetStrCurrentTime(string format= "%Y-%m-%d %H:%M:%S")=0;
		virtual tm  GetTmCurrentTime()=0;

		static string GetStrTime(time_t t,string format="%Y-%m-%d %H:%M:%S");
		/*
		��str װ��Ϊtm 
		*/
		static tm     GetTmFromStr(string time, string format = "%Y-%m-%d %H:%M:%S");
		/*
		��str ת��Ϊtime_t
		*/
		static time_t GetTimeFromStr(string time, string format = "%Y-%m-%d %H:%M:%S");
		tm   GetTmTime(time_t t);
	};
}
