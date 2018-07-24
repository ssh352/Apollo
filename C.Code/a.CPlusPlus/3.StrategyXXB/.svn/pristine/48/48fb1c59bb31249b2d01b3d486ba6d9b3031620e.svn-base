#include "stdafx.h"
#include "StrategyOnBar.h"
StrategyOnBar::StrategyOnBar()
{
	this->strategyid = 6666;
	this->strategyname = "testOnBar";
	this->subscribelist.push_back("jd1709");
	/*shared_ptr<AbstractTdTimeModel>  tdtmmodel(new TdTimeModel);
	kbar->OnInit(tdtmmodel);*/
}

void StrategyOnBar::OnTimeV(tm t)
{
}

void StrategyOnBar::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
}

void StrategyOnBar::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void StrategyOnBar::OnRtnDepthMarketDataV(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
}

void StrategyOnBar::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
}

void StrategyOnBar::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	
}

void StrategyOnBar::OnInitV()
{	
	//kbar->OnInit();
}

void StrategyOnBar::SetParameter(map<string, double> param) 
{
}

void StrategyOnBar::OnBar(TKDATA kbar)
{
	//LOG("OnBar_%s开%lf === 高%lf === 低%f === 收%lf === 量%lf",kbar.instrumentid,kbar.open,kbar.high,kbar.low,kbar.close,kbar.volume);
}

StrategyOnBar::~StrategyOnBar()
{
}
