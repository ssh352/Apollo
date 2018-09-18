#pragma once
#include<map>
#include<vector>
#include "ThostFtdcUserApiStruct.h"
using namespace std;

namespace Abstract {
	/*
	����������ṩ�˻�ȡ��Լ��̬��Ϣ����Ϣ�ķ���  �����Լ���� ��С�� �����·� ��
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