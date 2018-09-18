#pragma once
#include "AbstractStrategy.h"
#include "RecordData.h"
#include <time.h>
using namespace Abstract;
using namespace std;
/***钓鱼策略
A.判断开盘时为平开，涨跌幅小于2.5%。
B.挂上涨跌停板单。
C.若3分钟后无交易，撤单。
***/
class StrategyFlirt : public AbstractStrategy
{
public:
	StrategyFlirt();
	~StrategyFlirt();
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
	
};

