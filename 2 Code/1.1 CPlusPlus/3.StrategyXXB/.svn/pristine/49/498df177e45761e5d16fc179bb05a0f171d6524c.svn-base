#pragma once
#include<map>
#include<vector>
#include<memory>
#include "ThostFtdcUserApiStruct.h"
#include "BaseDataType.h"
using namespace std;

namespace Abstract {
	class BASELIB_API AbstractInstrumentFee
	{
	public:
		AbstractInstrumentFee();
		shared_ptr<vector<InstrumentFee>> virtual  LoadInstrumentFeefromLocalDB() = 0;//从数据库装置合约交易时间模版

		virtual ~AbstractInstrumentFee();
	protected:
		vector<InstrumentFee> instrumentfee;
		bool hasinit;
	};
}
