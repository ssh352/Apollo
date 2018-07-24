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
		void virtual  LoadInstrumentCommissionRatefromLocalDB() = 0;//�����ݿ�װ�ú�Լ����ʱ��ģ��

		virtual ~AbstractInstrumentCommissionRateMap();

		virtual CThostFtdcInstrumentCommissionRateField GetInstrumentCommisionRateByInstrumentID(string instrumentid)=0;

	protected:
		map<string, CThostFtdcInstrumentCommissionRateField> instrumentmap;
	
	};
}
