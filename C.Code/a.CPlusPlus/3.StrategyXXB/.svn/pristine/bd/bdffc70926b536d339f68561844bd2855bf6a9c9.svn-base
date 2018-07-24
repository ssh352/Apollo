#pragma once
#include "BaseDataType.h"
#include<memory>
#include<vector>
using namespace std;
namespace Abstract {
	//抽象的行情数据库接口
	 class BASELIB_API AbstractMDDB { //注意这个abstractdb 只能能在在oninit 方法里面调用
	public:		
		//采用绝对时间
		virtual	shared_ptr<vector<TKDATA>>  GetKdata(string dbname,string instrumentid, string starttime, string endtime, string period)=0;

	//采用相对时间
		virtual	shared_ptr<vector<TKDATA>>  GetKdata(string dbname, string instrumentid, int start, int  end, string period)=0;

	  //采用绝对时间
		virtual shared_ptr<vector<TKDATA>>  GetTick(string dbname,string instrumentid, string starttime, string endtime)=0;
	  //采用相对时间
		virtual shared_ptr<vector<TKDATA>>  GetTick(string dbname,string instrumentid, int start, int end)=0;
		//以TradingDay时间取
		virtual	shared_ptr<vector<TKDATA>>  GetKdataByTradingday(string dbname, string instrumentid, string starttime, string endtime, string period) = 0;
	};


}