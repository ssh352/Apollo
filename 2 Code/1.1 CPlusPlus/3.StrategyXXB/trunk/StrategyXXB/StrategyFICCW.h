#pragma once
#include "AbstractStrategy.h"
#include "LastTickMap.h""
using namespace Abstract;

class StrategyFICCW :public AbstractStrategy
{
public:
	StrategyFICCW();
	virtual ~StrategyFICCW();

protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, int nRequestID, bool bIsLast);
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, int nRequestID, bool bIsLast);
private:
	vector<string> GetPostionListFromFile(string files);
	bool IsNeedTradeLong();
	bool IsNeedTradeShort();
	bool IsInLongList(string instrumentid);  //是不是在多开列表
	bool IsInShortList(string instrumentid); //是不是在空开列表
	time_t StringToDatetime(string str);

	void TradeLong(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void TradeShort(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void PyRun();
private:
	bool needtradelong;//是否需要交易多
	bool needtradeshort;//是否需要交易空
	bool hascompletetradelong;//是否已经交易完毕多仓
	bool hascompletetradeshort; //是否已经交易完毕空仓
	char lastradelongday[9]; //最后执行交易的日期
	char lastradeshortday[9]; //最后执行交易的日期
	double singglemount; //单品种交易金额
	time_t lasttradingtime;
	time_t lastpythonruntime;
	vector<string> list_longpositon; //调仓
	vector<string> list_shortpositon;

};
