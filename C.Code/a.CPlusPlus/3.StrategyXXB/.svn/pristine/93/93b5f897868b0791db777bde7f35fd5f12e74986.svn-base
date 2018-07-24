#pragma once
#include<map>
#include<vector>
#include<memory>
#include "ThostFtdcUserApiStruct.h"
#include "BaseDataType.h"
using namespace std;

namespace Abstract {
	class BASELIB_API AbstractInstrumentCommissionRateMap
	{
	public:
		AbstractInstrumentCommissionRateMap();
		void virtual  LoadInstrumentCommissionRatefromLocalDB() = 0;//从数据库装置合约交易时间模版

		virtual ~AbstractInstrumentCommissionRateMap();

		virtual CThostFtdcInstrumentCommissionRateField GetInstrumentCommisionRateByInstrumentID(string instrumentid)=0;

	protected:
		map<string, CThostFtdcInstrumentCommissionRateField> instrumentmap;
	
	};
}
