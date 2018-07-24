#pragma once
#include "AbstractStrategy.h"
#include "RecordData.h"
#include <time.h>
using namespace Abstract;
using namespace std;
/***塑料类40日均线提醒***/
class StrategyMA40Alert : public AbstractStrategy
{
public:
	StrategyMA40Alert();
	~StrategyMA40Alert();
protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnTime(tm t);
	virtual void OnInit();

public:
	map<CString, RecordData> m_RecordData;

public:
	int flag;
};
