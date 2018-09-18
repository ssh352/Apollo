#include "stdafx.h"
#include "StrategyCoffe.h"
#include "math.h"
StrategyCoffe::StrategyCoffe()
{
	this->strategyid = 1008;
	this->strategyname = "coffe13";
	//this->subscribelist.push_back("SPC y1705&p1705");
	this->subscribelist.push_back("m1609");
	this->subscribelist.push_back("p1705");
	this->paralist.push_back("no prameter");
	
	//this->subscribelist.push_back("p1705");
	//this->isopen = false;
}


StrategyCoffe::~StrategyCoffe()
{
}
void StrategyCoffe::OnRtnTrade(CThostFtdcTradeField *pTrade) {

}//成交回报		
void StrategyCoffe::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {


 }
void StrategyCoffe::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {

	this->indicators["test"] = 99 * rand() / 100;
	this->indicators["test1"] = 1000;
	if (this->portfoliomanager->GetPostionDetailCount() == 0 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open) == 0) 
	{
		CThostFtdcInputOrderField inputOrderField1;

		//下涨停板卖开订单
		memset(&inputOrderField1, 0, sizeof(inputOrderField1));
		strcpy(inputOrderField1.BrokerID, this->brokerid.c_str());
		strcpy(inputOrderField1.InvestorID, this->investid.c_str());
		strcpy(inputOrderField1.InstrumentID, pDepthMarketData->InstrumentID);
		sprintf(inputOrderField1.OrderRef, "%s", this->GetNewOrderRef().c_str());
		///报单价格条件
		inputOrderField1.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///买卖方向
		inputOrderField1.Direction = THOST_FTDC_D_Sell;

		///组合开平标志
		inputOrderField1.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;
		///组合投机套保标志
		inputOrderField1.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
		inputOrderField1.LimitPrice = pDepthMarketData->BidPrice1;
		inputOrderField1.VolumeTotalOriginal = 1;
		///有效期类型: 当日有效
		inputOrderField1.TimeCondition = THOST_FTDC_TC_GFD;
		///成交量类型: 任何数量
		inputOrderField1.VolumeCondition = THOST_FTDC_VC_AV;
		///最小成交量: 1
		inputOrderField1.MinVolume = 1;
		///触发条件: 立即
		inputOrderField1.ContingentCondition = THOST_FTDC_CC_Immediately;
		///止损价
		///inputOrderField    StopPrice;
		///强平原因: 非强平
		inputOrderField1.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///自动挂起标志: 否
		inputOrderField1.IsAutoSuspend = 0;

		///用户强评标志: 否
		inputOrderField1.UserForceClose = 0;
		int i = this->tdapi->ReqOrderInsert(&inputOrderField1, this->requestid++);

	}

	if (this->portfoliomanager->GetPostionDetailCount() == 1 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, THOST_FTDC_OF_Close) == 0) {

		CThostFtdcInputOrderField inputOrderField1;

		//下涨停板卖开订单
		memset(&inputOrderField1, 0, sizeof(inputOrderField1));
		strcpy(inputOrderField1.BrokerID, this->brokerid.c_str());
		strcpy(inputOrderField1.InvestorID, this->investid.c_str());
		strcpy(inputOrderField1.InstrumentID, pDepthMarketData->InstrumentID);
		sprintf(inputOrderField1.OrderRef, "%s", this->GetNewOrderRef().c_str());
		///报单价格条件
		inputOrderField1.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///买卖方向
		inputOrderField1.Direction = THOST_FTDC_D_Buy;

		///组合开平标志
		inputOrderField1.CombOffsetFlag[0] = THOST_FTDC_OFEN_Close;
		///组合投机套保标志
		inputOrderField1.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
		inputOrderField1.LimitPrice = pDepthMarketData->BidPrice1;
		inputOrderField1.VolumeTotalOriginal = 1;
		///有效期类型: 当日有效
		inputOrderField1.TimeCondition = THOST_FTDC_TC_GFD;
		///成交量类型: 任何数量
		inputOrderField1.VolumeCondition = THOST_FTDC_VC_AV;
		///最小成交量: 1
		inputOrderField1.MinVolume = 1;
		///触发条件: 立即
		inputOrderField1.ContingentCondition = THOST_FTDC_CC_Immediately;
		///止损价
		///inputOrderField    StopPrice;
		///强平原因: 非强平
		inputOrderField1.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///自动挂起标志: 否
		inputOrderField1.IsAutoSuspend = 0;

		///用户强评标志: 否
		inputOrderField1.UserForceClose = 0;
		int i = this->tdapi->ReqOrderInsert(&inputOrderField1, this->requestid++);

	}




}//行情接收函数
void  StrategyCoffe::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {



 }
void  StrategyCoffe::OnRtnOrder(CThostFtdcOrderField *pOrder) {


 }
void StrategyCoffe::OnInit() {

}

 void StrategyCoffe::OnTime(tm t)
{

}