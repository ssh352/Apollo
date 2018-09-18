#pragma once
#include "BaseLib.h"
#include <string>
#include <map>
#include<vector>
#include<set>
#include "ThostFtdcUserApiStruct.h"
using namespace std;
//�������������¼���һ����
namespace Abstract {
	class BASELIB_API LastTickMap {
	public:
		void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField ticket);
		CThostFtdcDepthMarketDataField getLastTick(string instrumentid);
		bool IsAllTickReady(vector<string> &vec_instrument); //�б���ĺ�Լ�ļ۸��Ƿ�׼������
		bool IsAllTickReady(set<string> &set_instrument);
		void Clear();//��ռ۸� ������ʱ������л�
		LastTickMap();
	private:
		map<string, CThostFtdcDepthMarketDataField> instrumentpricemap;

 };



}