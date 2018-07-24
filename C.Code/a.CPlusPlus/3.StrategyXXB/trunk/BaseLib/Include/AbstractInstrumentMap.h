#pragma once
#include<map>
#include<vector>
#include "ThostFtdcUserApiStruct.h"
using namespace std;

namespace Abstract {
	/*
	这个抽象类提供了获取合约静态信息的信息的方法  比如合约乘数 最小跳 交割月份 等
	*/
	 class BASELIB_API AbstactInstrumentMap {
	 public:
		virtual ~AbstactInstrumentMap();
		AbstactInstrumentMap();
		bool virtual HasInstrument(string Instrumentid)=0;
		virtual CThostFtdcInstrumentField  getInstrumentid(string instrumentid) = 0;
	
	protected:
		map<string, CThostFtdcInstrumentField> instrumentmap;
		bool hasinit;
	};


}