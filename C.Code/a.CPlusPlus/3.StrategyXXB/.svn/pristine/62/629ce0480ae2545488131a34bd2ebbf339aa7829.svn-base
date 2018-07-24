#pragma once
#include "AbstractBarStrategy.h"
#include "KBarGenerater1.h"
using namespace Abstract;
class StrategyOnBar :public AbstractBarStrategy {

public:
	StrategyOnBar();
	virtual ~StrategyOnBar();
protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketDataV(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInitV();
	virtual void OnTimeV(tm t);
	virtual void OnBar(TKDATA kbar);
	virtual void SetParameter(map<string, double> param);//设置参数
};

