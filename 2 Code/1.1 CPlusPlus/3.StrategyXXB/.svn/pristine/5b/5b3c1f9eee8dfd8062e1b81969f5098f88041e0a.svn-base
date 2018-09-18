#pragma once
#include "AbstractStrategy.h"
#include "LastTickMap.h"
using namespace Abstract;
class StrategyArbitrageNotice :public AbstractStrategy
{
public:
	StrategyArbitrageNotice();
	virtual ~StrategyArbitrageNotice();
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
	string ru01;
	string ru05;
	string ru09;
	string sr01;
	string sr05;
	string sr09;
	string ta01;
	string ta05;
	string ta09;
	string pp01;
	string pp09;
	string m01;
	string m05;
	string m09;
	string rm01;
	string rm05;
	string rm09;

};