#pragma once
#include "AbstractStrategy.h"
#include "LastTickMap.h"
using namespace Abstract;
class StrategyGrid :public AbstractStrategy
{
public:
	StrategyGrid();
	virtual ~StrategyGrid();
protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
private:
	LastTickMap* lasttickmap;
	string lag1; //一腿
	string lag2; //两腿
	double centre; //中枢
	double gridgap; //网格间距
	

};