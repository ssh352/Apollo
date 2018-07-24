#pragma once
#include "AbstractStrategy.h"
#include "LastTickMap.h"
using namespace Abstract;
class StrategyPostpone:public AbstractStrategy
{
public:
	StrategyPostpone();
	virtual ~StrategyPostpone();
private:
	LastTickMap *lasttickmap;
	

protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
};

