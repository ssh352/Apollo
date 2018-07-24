#pragma once
#include "AbstractBarStrategy.h"
#include "RecordData.h"
#include <time.h>
using namespace Abstract;
using namespace std;
/***均线趋势策略

***/
class StrategyTestMTOrder : public AbstractBarStrategy
{
public:
	StrategyTestMTOrder();
	~StrategyTestMTOrder();
protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketDataV(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnBar(TKDATA kbar);
	virtual void OnTimeV(tm t);
	virtual void OnInitV();

};