#pragma once
#include "AbstractStrategy.h"
#include "LastTickMap.h"
#include "OrderExcuter.h"
using namespace Abstract;

class StrategyFICCN:public AbstractStrategy
{
public:
	StrategyFICCN();
	virtual ~StrategyFICCN();

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
	void PyRun();
	void OnCommand(string path, string command, string parameter);
private:	
	double singglemount; //单品种交易金额
	time_t lasttradingtime;
	OrderExcuter* orderexcuter; //执行order
	LastTickMap* lasttickmap;//lasttickmap
	vector<string> vec_longpostion;
	vector<string> vec_shortpostion;
	set<string> set_longpostion;
	set<string> set_shortpostion;
};
