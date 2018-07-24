#include "stdafx.h"
#include <memory>
#include "StrategyGrid.h"
using namespace Abstract;
StrategyGrid::StrategyGrid()
{
	this->strategyid = 1100;
	this->strategyname = "套利提醒";
	this->lag1 = "c1801";
	this->lag2 = "c1805";
	this->centre = -44.00;
    this->gridgap = 5;  
	this->subscribelist.push_back(lag1);
	this->subscribelist.push_back(lag2);
	this->lasttickmap = new LastTickMap();
}
StrategyGrid::~StrategyGrid()
{
	delete this->lasttickmap;

}
void StrategyGrid::OnRtnTrade(CThostFtdcTradeField *pTrade)
{

}
void StrategyGrid::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void StrategyGrid::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	if (lasttickmap->getLastTick(lag1).LastPrice > 0 && lasttickmap->getLastTick(lag2).LastPrice > 0) {
		int i = (lasttickmap->getLastTick(lag1).LastPrice - lasttickmap->getLastTick(lag2).LastPrice - this->centre) / this->gridgap;

		if (i > 0) {

			//卖lag1 份数i 买lag1 份数 i
			int volume = i - this->portfoliomanager->GetPostionDetailCount(lag1, THOST_FTDC_D_Sell)-this->portfoliomanager->GetOrderInputCount(lag1, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open) - this->portfoliomanager->GetOrderCount(lag2, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open);;
			if (volume > 0) { //需要补单
				CThostFtdcInputOrderField orderinput=	this->CreateOrder((char*)lag1.c_str(), lasttickmap->getLastTick(lag1).LowerLimitPrice, volume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD,true);				
				this->tdapi->ReqOrderInsert(&orderinput,this->requestid++);
			}
			if (volume < 0) { //平掉一些
				CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)lag1.c_str(), lasttickmap->getLastTick(lag1).LowerLimitPrice, volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD,false);
				this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
			}
			int volume1 = i - this->portfoliomanager->GetPostionDetailCount(lag2, THOST_FTDC_D_Buy)-this->portfoliomanager->GetOrderInputCount(lag2, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open)-this->portfoliomanager->GetOrderCount(lag2, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open);
			if (volume1 > 0) {
				CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)lag2.c_str(), lasttickmap->getLastTick(lag2).UpperLimitPrice, volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD,true);
				this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
			}
			if (volume1 < 0) {
				CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)lag2.c_str(), lasttickmap->getLastTick(lag2).UpperLimitPrice, volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD,false);
				this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
			}

		}

		//	this->logger->Log("%s-%s 价差 %f ", sr01.c_str(), sr05.c_str(), lasttickmap->getLastTick(sr01).LastPrice - lasttickmap->getLastTick(sr05).LastPrice);



	}
	//if(this->in)
}
void  StrategyGrid::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void  StrategyGrid::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

}
void StrategyGrid::OnInit()
{

}
void StrategyGrid::OnTime(tm t)
{

	
}