#include "stdafx.h"
#include "StrategyPostpone.h"
#include "LOGGER.h"
#include "time.h"
//#include "Tyloodbapiclientlib.h"
using namespace CTPTOOL;


StrategyPostpone::StrategyPostpone()
{
	this->strategyid = 1002;
	this->strategyname = "PostPone";
	this->subscribelist.push_back("c1805");
	this->subscribelist.push_back("cs1805");
	this->paralist.push_back("�����ŶӶ��е�λ���µ�");
	lasttickmap = new LastTickMap();
}

void StrategyPostpone::OnInit() {

	
	string  endtime = this->timer->GetStrCurrentTime("%Y-%m-%d %H:%M:%S");
	time_t  endtimet = this->timer->GetTCurrentTime();
	time_t   starttimet = endtimet - 60 * 60 * 24 * 20;   //10��ǰ ��ô���� 60*60*24*10
	tm *local = localtime(&starttimet);
	char str[80];
	strftime(str, 80, "%Y-%m-%d %H:%M:%S", local);
	shared_ptr<vector<TKDATA>> list_k = this->mddb->GetKdata("WIND", "cs1501", str, endtime, "5m");

}
void StrategyPostpone::OnTime(tm t) {

	if (t.tm_hour == 14 && t.tm_min == 15) {

		CThostFtdcInputOrderField inputOrderField = this->CreateOrder("cs1501", lasttickmap->getLastTick("cs1801").LowerLimitPrice, 10, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);

		this->tdapi->ReqOrderInsert(&inputOrderField,this->requestid++);

		//this->portfoliomanager->

	}

	//this->parampersistencer->AddOrUpdataParameter("asdfadf", 1.000);

	//this->portfoliomanager->GetPostion("c")


}
StrategyPostpone::~StrategyPostpone()
{
	delete this->lasttickmap;

}
void StrategyPostpone::OnRtnTrade(CThostFtdcTradeField *pTrade) {


	

}
void StrategyPostpone::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

		LOGGER::CTP_Log("����ʧ�� ");
}
void StrategyPostpone::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	
	lasttickmap->OnRtnDepthMarketData(*pDepthMarketData);

}
void  StrategyPostpone::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	
}
void  StrategyPostpone::OnRtnOrder(CThostFtdcOrderField *pOrder) {
}