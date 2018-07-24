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

}//�ɽ��ر�		
void StrategyCoffe::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {


 }
void StrategyCoffe::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {

	this->indicators["test"] = 99 * rand() / 100;
	this->indicators["test1"] = 1000;
	if (this->portfoliomanager->GetPostionDetailCount() == 0 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open) == 0) 
	{
		CThostFtdcInputOrderField inputOrderField1;

		//����ͣ����������
		memset(&inputOrderField1, 0, sizeof(inputOrderField1));
		strcpy(inputOrderField1.BrokerID, this->brokerid.c_str());
		strcpy(inputOrderField1.InvestorID, this->investid.c_str());
		strcpy(inputOrderField1.InstrumentID, pDepthMarketData->InstrumentID);
		sprintf(inputOrderField1.OrderRef, "%s", this->GetNewOrderRef().c_str());
		///�����۸�����
		inputOrderField1.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///��������
		inputOrderField1.Direction = THOST_FTDC_D_Sell;

		///��Ͽ�ƽ��־
		inputOrderField1.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;
		///���Ͷ���ױ���־
		inputOrderField1.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
		inputOrderField1.LimitPrice = pDepthMarketData->BidPrice1;
		inputOrderField1.VolumeTotalOriginal = 1;
		///��Ч������: ������Ч
		inputOrderField1.TimeCondition = THOST_FTDC_TC_GFD;
		///�ɽ�������: �κ�����
		inputOrderField1.VolumeCondition = THOST_FTDC_VC_AV;
		///��С�ɽ���: 1
		inputOrderField1.MinVolume = 1;
		///��������: ����
		inputOrderField1.ContingentCondition = THOST_FTDC_CC_Immediately;
		///ֹ���
		///inputOrderField    StopPrice;
		///ǿƽԭ��: ��ǿƽ
		inputOrderField1.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///�Զ������־: ��
		inputOrderField1.IsAutoSuspend = 0;

		///�û�ǿ����־: ��
		inputOrderField1.UserForceClose = 0;
		int i = this->tdapi->ReqOrderInsert(&inputOrderField1, this->requestid++);

	}

	if (this->portfoliomanager->GetPostionDetailCount() == 1 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, THOST_FTDC_OF_Close) == 0) {

		CThostFtdcInputOrderField inputOrderField1;

		//����ͣ����������
		memset(&inputOrderField1, 0, sizeof(inputOrderField1));
		strcpy(inputOrderField1.BrokerID, this->brokerid.c_str());
		strcpy(inputOrderField1.InvestorID, this->investid.c_str());
		strcpy(inputOrderField1.InstrumentID, pDepthMarketData->InstrumentID);
		sprintf(inputOrderField1.OrderRef, "%s", this->GetNewOrderRef().c_str());
		///�����۸�����
		inputOrderField1.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///��������
		inputOrderField1.Direction = THOST_FTDC_D_Buy;

		///��Ͽ�ƽ��־
		inputOrderField1.CombOffsetFlag[0] = THOST_FTDC_OFEN_Close;
		///���Ͷ���ױ���־
		inputOrderField1.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
		inputOrderField1.LimitPrice = pDepthMarketData->BidPrice1;
		inputOrderField1.VolumeTotalOriginal = 1;
		///��Ч������: ������Ч
		inputOrderField1.TimeCondition = THOST_FTDC_TC_GFD;
		///�ɽ�������: �κ�����
		inputOrderField1.VolumeCondition = THOST_FTDC_VC_AV;
		///��С�ɽ���: 1
		inputOrderField1.MinVolume = 1;
		///��������: ����
		inputOrderField1.ContingentCondition = THOST_FTDC_CC_Immediately;
		///ֹ���
		///inputOrderField    StopPrice;
		///ǿƽԭ��: ��ǿƽ
		inputOrderField1.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///�Զ������־: ��
		inputOrderField1.IsAutoSuspend = 0;

		///�û�ǿ����־: ��
		inputOrderField1.UserForceClose = 0;
		int i = this->tdapi->ReqOrderInsert(&inputOrderField1, this->requestid++);

	}




}//������պ���
void  StrategyCoffe::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {



 }
void  StrategyCoffe::OnRtnOrder(CThostFtdcOrderField *pOrder) {


 }
void StrategyCoffe::OnInit() {

}

 void StrategyCoffe::OnTime(tm t)
{

}