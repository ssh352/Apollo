#pragma once
#include "BaseLib.h"
#include <string>
#include <map>
#include<vector>
#include<set>
#include "ThostFtdcUserApiStruct.h"
using namespace std;
//这个类是用来记录最近一跳的
namespace Abstract {
	class BASELIB_API LastTickMap {
	public:
		void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField ticket);
		CThostFtdcDepthMarketDataField getLastTick(string instrumentid);
		bool IsAllTickReady(vector<string> &vec_instrument); //列表里的合约的价格是否都准备好了
		bool IsAllTickReady(set<string> &set_instrument);
		void Clear();//清空价格 当换日时做这个切换
		LastTickMap();
	private:
		map<string, CThostFtdcDepthMarketDataField> instrumentpricemap;

 };



}