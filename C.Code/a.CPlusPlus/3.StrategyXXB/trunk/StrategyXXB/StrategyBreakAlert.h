#pragma once
#include "AbstractStrategy.h"
#include "KBarGenerater.h"

using namespace Abstract;
using namespace std;
/***突破报警策略
A.计算10k、20k、30k的高低价。
B.判断是否是平台。
C.若是平台，则突破时报警。
***/
class StrategyBreakAlert : public AbstractStrategy
{
public:
	StrategyBreakAlert();
	~StrategyBreakAlert();

private:
	KBarGenerater * kbargen;
	int flag;
	double LasttickVolume;			//上一秒的量
	CString AlertTime;

protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
};