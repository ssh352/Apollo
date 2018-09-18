#include "stdafx.h"
#include <memory>
#include "StrategyArbitrageNotice.h"
using namespace Abstract;
StrategyArbitrageNotice::StrategyArbitrageNotice()
{   
	this->strategyid = 7000;
	this->strategyname = "套利提醒";
	this->ru01 = "ru1801";
	this->ru05 = "ru1805";
	this->ru09 = "ru1809";	
	this->sr01 = "SR801";
	this->sr05 = "SR805";
	this->sr09 = "SR809";
	this->ta01 = "TA801";
	this->ta05 = "TA805";
	this->ta09 = "TA809";
	this->pp01 = "pp1901";
	this->pp09 = "pp1809";
	this->m01 = "m1801";
	this->m05 = "m1805";
	this->m09 = "m1809";
	this->rm01 = "RM801";
	this->rm05 = "RM805";
	this->rm09 = "RM809";
	this->subscribelist.push_back(ru01);
	this->subscribelist.push_back(ru05);
	this->subscribelist.push_back(ru09);
	this->subscribelist.push_back(sr01);
	this->subscribelist.push_back(sr05);
	this->subscribelist.push_back(sr09);
	this->subscribelist.push_back(ta01);
	this->subscribelist.push_back(ta05);
	this->subscribelist.push_back(ta09);

	this->subscribelist.push_back(pp01);
	this->subscribelist.push_back(pp09);
	
	this->subscribelist.push_back(m01);
	this->subscribelist.push_back(m05);
	this->subscribelist.push_back(m09);

	this->subscribelist.push_back(rm01);
	this->subscribelist.push_back(rm05);
	this->subscribelist.push_back(rm09);


	this->lasttickmap = new LastTickMap();
}
StrategyArbitrageNotice::~StrategyArbitrageNotice()
{
	delete this->lasttickmap;

}
void StrategyArbitrageNotice::OnRtnTrade(CThostFtdcTradeField *pTrade)
{

	 }
void StrategyArbitrageNotice::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

	 }
void StrategyArbitrageNotice::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	this->lasttickmap->OnRtnDepthMarketData(*pDepthMarketData);
	//  ru
	if (lasttickmap->getLastTick(ru01).LastPrice > 0 && lasttickmap->getLastTick(ru05).LastPrice > 0) {
		if (lasttickmap->getLastTick(ru01).LastPrice - lasttickmap->getLastTick(ru05).LastPrice<-450) {
			this->logger->Log("%s-%s 价差 %f ",ru01.c_str(),ru05.c_str(), lasttickmap->getLastTick(ru01).LastPrice - lasttickmap->getLastTick(ru05).LastPrice);
		}
	}


	if (lasttickmap->getLastTick(ru05).LastPrice > 0 && lasttickmap->getLastTick(ru09).LastPrice > 0) {
		if (lasttickmap->getLastTick(ru05).LastPrice - lasttickmap->getLastTick(ru09).LastPrice<-450) {
			this->logger->Log("%s-%s 价差 %f ",ru05.c_str(),ru09.c_str(), lasttickmap->getLastTick(ru05).LastPrice - lasttickmap->getLastTick(ru09).LastPrice);
		}
	}



	//sr


	if (lasttickmap->getLastTick(sr01).LastPrice > 0 && lasttickmap->getLastTick(sr05).LastPrice > 0) {
		if (lasttickmap->getLastTick(sr01).LastPrice - lasttickmap->getLastTick(sr05).LastPrice<-150) {
			this->logger->Log("%s-%s 价差 %f ", sr01.c_str(), sr05.c_str(), lasttickmap->getLastTick(sr01).LastPrice - lasttickmap->getLastTick(sr05).LastPrice);
		}
	}

	if (lasttickmap->getLastTick(sr05).LastPrice > 0 && lasttickmap->getLastTick(sr09).LastPrice > 0) {
		if (lasttickmap->getLastTick(sr05).LastPrice - lasttickmap->getLastTick(sr09).LastPrice<-150) {
			this->logger->Log("%s-%s 价差 %f ", sr05.c_str(), sr09.c_str(), lasttickmap->getLastTick(sr05).LastPrice - lasttickmap->getLastTick(sr09).LastPrice);
		}
	}

	//pp
	if (lasttickmap->getLastTick(pp09).LastPrice > 0 && lasttickmap->getLastTick(pp01).LastPrice > 0) {
		if (lasttickmap->getLastTick(pp09).LastPrice - lasttickmap->getLastTick(pp01).LastPrice<-150) {
			this->logger->Log("%s-%s 价差 %f ", pp09.c_str(), pp01.c_str(), lasttickmap->getLastTick(pp09).LastPrice - lasttickmap->getLastTick(pp01).LastPrice);
		}
	}
	//m-rm

	if (lasttickmap->getLastTick(m01).LastPrice > 0 && lasttickmap->getLastTick(rm01).LastPrice > 0) {
		if (lasttickmap->getLastTick(m01).LastPrice - lasttickmap->getLastTick(rm01).LastPrice<400) {
			this->logger->Log("%s-%s 价差 %f ", m01.c_str(), rm01.c_str(), lasttickmap->getLastTick(m01).LastPrice - lasttickmap->getLastTick(rm01).LastPrice);
		}
	}

	if (lasttickmap->getLastTick(m05).LastPrice > 0 && lasttickmap->getLastTick(rm05).LastPrice > 0) {
		if (lasttickmap->getLastTick(m05).LastPrice - lasttickmap->getLastTick(rm05).LastPrice<400) {
			this->logger->Log("%s-%s 价差 %f ", m05.c_str(), rm05.c_str(), lasttickmap->getLastTick(m05).LastPrice - lasttickmap->getLastTick(rm05).LastPrice);
		}
	}


	if (lasttickmap->getLastTick(m09).LastPrice > 0 && lasttickmap->getLastTick(rm09).LastPrice > 0) {
		if (lasttickmap->getLastTick(m09).LastPrice - lasttickmap->getLastTick(rm09).LastPrice<400) {
			this->logger->Log("%s-%s 价差 %f ", m09.c_str(), rm09.c_str(), lasttickmap->getLastTick(m09).LastPrice - lasttickmap->getLastTick(rm09).LastPrice);
		}
	}


	//if(this->in)
}
void  StrategyArbitrageNotice::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void  StrategyArbitrageNotice::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

	 }
void StrategyArbitrageNotice::OnInit()
{

	 }
void StrategyArbitrageNotice::OnTime(tm t)
{

	if (t.tm_mon == 7 && t.tm_mday >= 13 && t.tm_hour == 9 && t.tm_min == 0 && t.tm_sec == 0) {

		this->logger->Log("ru9-1 11月仓单注销关注反套机会");
		this->logger->Log("sr9-1 11月仓单注销关注反套机会");
	}
	if (t.tm_mon == 3 && t.tm_mday >= 13&&t.tm_hour==9&&t.tm_min==0&&t.tm_sec==0) {

		this->logger->Log("FG5-9 5月仓单注销关注反套机会");
		this->logger->Log("OI5-9 5月仓单注销关注反套机会");
	}
	 }