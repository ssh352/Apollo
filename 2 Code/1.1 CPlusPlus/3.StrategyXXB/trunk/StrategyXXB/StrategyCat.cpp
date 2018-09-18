#include "stdafx.h"
#include "StrategyCat.h"
#include "LOGGER.h"
#include "ta_func.h"
#include "time.h"
//#include "Tyloodbapiclientlib.h"
using namespace CTPTOOL;


StrategyCat::StrategyCat()
{
	this->strategyid = 1002;
	this->strategyname = "cat";
	this->symbol = "cs1801";
	this->subscribelist.push_back(this->symbol);
	this->paralist.push_back("�����ŶӶ��е�λ���µ�");
	this->postion = 10000;   
	this->kbargen = new KBarGenerater1();
	this->kbargen->OnInit(this->symbol);
}
void StrategyCat::OnBar(TKDATA kdata)
{
	//
	



}
void StrategyCat::OnInit() {

	this->kbargen->OnInit(this->symbol);
	string  endtime = this->timer->GetStrCurrentTime("%Y-%m-%d %H:%M:%S");
	time_t  endtimet = this->timer->GetTCurrentTime();
	time_t   starttimet = endtimet - 60 * 60 * 24 * 20;   //10��ǰ ��ô���� 60*60*24*10
	tm *local = localtime(&starttimet);
	char str[80];
	strftime(str, 80, "%Y-%m-%d %H:%M:%S", local);
	shared_ptr<vector<TKDATA>> list_k = this->mddb->GetKdata("WIND", this->symbol, str, endtime, "5m");
	this->kbargen->MergerData(list_k, PeriodType::PERIOD_M5, true); //��30���ӵ�k�����뵽k����������
	//->mddb->GetKdata("WIND",)
	this->kbargen->pf_onbar= std::bind(&StrategyCat::OnBar, this, std::placeholders::_1);
}
void StrategyCat::OnTime(tm t) {

	//LOGGER::CTP_Log("ontime %d%d ",t.tm_min,t.tm_sec);
}
StrategyCat::~StrategyCat()
{

	delete kbargen;

}
void StrategyCat::OnRtnTrade(CThostFtdcTradeField *pTrade) {


	if (pTrade->OffsetFlag== THOST_FTDC_OFEN_Open) {  //���û���µ�����û�йҵ� ���Ҹճɽ��ĵ����ǿ���
		CThostFtdcInputOrderField inputOrderField;
		(void)memset(&inputOrderField, 0, sizeof(inputOrderField));
		strcpy(inputOrderField.BrokerID, this->brokerid.c_str());
		strcpy(inputOrderField.InvestorID, this->investid.c_str());
		strcpy(inputOrderField.InstrumentID, pTrade->InstrumentID);
		sprintf(inputOrderField.OrderRef, "%s", this->GetNewOrderRef().c_str());
		CString instrumentid;
		instrumentid.Format("%s", pTrade->InstrumentID);
		double pricetick =this->instrumentmap->getInstrumentid(instrumentid.GetBuffer(0)).PriceTick;
		///�����۸�����
		inputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///��������
		inputOrderField.Direction = THOST_FTDC_D_Sell;
		///��Ͽ�ƽ��־
		inputOrderField.CombOffsetFlag[0] = THOST_FTDC_OFEN_Close;
		///���Ͷ���ױ���־
		inputOrderField.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
		inputOrderField.LimitPrice = pTrade->Price - 1.00*pricetick;//��һ��ֹӯ
		inputOrderField.VolumeTotalOriginal = 1;
		///��Ч������: ������Ч
		inputOrderField.TimeCondition = THOST_FTDC_TC_IOC;
		///�ɽ�������: �κ�����
		inputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
		///��С�ɽ���: 1
		inputOrderField.MinVolume = 1;
		///��������: ����
		inputOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
		///ֹ���
		///inputOrderField    StopPrice;
		///ǿƽԭ��: ��ǿƽ
		inputOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///�Զ������־: ��
		inputOrderField.IsAutoSuspend = 0;
		///�û�ǿ����־: ��
		inputOrderField.UserForceClose = 0;

		int i = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
		

	}
	


}
void StrategyCat::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

	LOGGER::CTP_Log("����ʧ�� ");
 }
void StrategyCat::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	kbargen->OnRtnDepthMarketData(pDepthMarketData);
	deque<TKDATA> deque_list=this->kbargen->GetBarList(PeriodType::PERIOD_M5,200);
	double a[200];
	memset(a, 0, sizeof(a));

	for (int i = 0; i < 200; i++) {

	}



	TA_Integer outBeg;
	TA_Integer outNbElement;
	string actionday = pDepthMarketData->ActionDay;
	string updatetime = pDepthMarketData->UpdateTime;
	string time_string = actionday +" " + updatetime;
	//LOG("%s",time_string.c_str());
	struct tm tm1;
	time_t time1;
	int i = sscanf(time_string.c_str(), "%04d%02d%02d %02d:%02d:%02d",
		&(tm1.tm_year),
		&(tm1.tm_mon),
		&(tm1.tm_mday),
		&(tm1.tm_hour),
		&(tm1.tm_min),
		&(tm1.tm_sec));
	tm1.tm_year = tm1.tm_year - 1900;
	time1 = mktime(&tm1);

//	LOGGER::CTP_Log("cat δ�ɽ��Ĺҵ� %d", this->portfoliomanager->GetOrderCount());
//	LOGGER::CTP_Log("cat δ�ɽ��Ŀ��ֹҵ� %s %d",pDepthMarketData->InstrumentID, this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID), THOST_FTDC_D_Buy, THOST_FTDC_OF_Open);

	//����Ƕ൥ ������ּ۸� �����ڵļ۸����10�� ����
	

	if (this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open) > 0) {
     //����		
		vector<CThostFtdcOrderField> list = this->portfoliomanager->GetOrder(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open);
		for (int i = 0; i < list.size(); i++) {
			double pricetick = this->instrumentmap->getInstrumentid(list[i].InstrumentID).PriceTick;
			if ((list[i].LimitPrice < pDepthMarketData->BidPrice1 - 5.00*pricetick)|| (list[i].LimitPrice == pDepthMarketData->BidPrice1&&(pDepthMarketData->BidVolume1<4 * pDepthMarketData->AskVolume1||pDepthMarketData->BidVolume1<300))){//����ҵ��ļ۸�С��bidprice 10�� ��ô��������ҵ�
				CThostFtdcInputOrderActionField inputOrderActionField;
				(void)memset(&inputOrderActionField, 0, sizeof(inputOrderActionField));
				strcpy(inputOrderActionField.BrokerID, this->brokerid.c_str());
				strcpy(inputOrderActionField.InvestorID, this->investid.c_str());
				strcpy(inputOrderActionField.OrderRef, list[i].OrderRef);
				inputOrderActionField.FrontID = list[i].FrontID;
				inputOrderActionField.SessionID = list[i].SessionID;
				//inputOrderActionField.OrderActionRef=0;
				inputOrderActionField.ActionFlag = THOST_FTDC_AF_Delete;
				strcpy(inputOrderActionField.InstrumentID, list[i].InstrumentID);
				int i = this->tdapi->ReqOrderAction(&inputOrderActionField, this->requestid++);
				if(list[i].LimitPrice < pDepthMarketData->BidPrice1 - 4.00*pricetick)
				LOGGER::CTP_Log("�۸�ƫ����� �������ֹҵ�");
				else
				LOGGER::CTP_Log("�����ڹҵ����Ƴ���");		

				//this->logger->Log()
			}
	      
		} 
	}


	if (this->portfoliomanager->GetOrderCount()==0&&this->portfoliomanager->GetPostionDetailCount()==0&& this->portfoliomanager->GetOrderInputCount()==0) { //�޸ĳ� ����µ�����û��size �ȴ��ɽ��Ķ���Ҳû��
		LOGGER::CTP_Log("��ʼ�µ�");
		//if (pDepthMarketData->SettlementPrice <= pDepthMarketData->BidPrice1)
		//{


			//return;
		//}
			
		CThostFtdcInputOrderField inputOrderField;
		(void)memset(&inputOrderField, 0, sizeof(inputOrderField));
		strcpy(inputOrderField.BrokerID, this->brokerid.c_str());
		strcpy(inputOrderField.InvestorID, this->investid.c_str());
		strcpy(inputOrderField.InstrumentID, pDepthMarketData->InstrumentID);
		double pricetick = this->instrumentmap->getInstrumentid(pDepthMarketData->InstrumentID).PriceTick;
		sprintf(inputOrderField.OrderRef, "%s", this->GetNewOrderRef().c_str());
		///�����۸�����
		inputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///��������
		inputOrderField.Direction = THOST_FTDC_D_Buy;
		///��Ͽ�ƽ��־
		inputOrderField.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;
		///���Ͷ���ױ���־
		inputOrderField.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
		inputOrderField.LimitPrice = pDepthMarketData->AskPrice1+5.0*pricetick; //����ƫ��5�����ȴ�
		inputOrderField.VolumeTotalOriginal = 1;
		///��Ч������: ������Ч
		inputOrderField.TimeCondition = THOST_FTDC_TC_GFD;
		///�ɽ�������: �κ�����
		inputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
		///��С�ɽ���: 1
		inputOrderField.MinVolume = 1;
		///��������: ����
		inputOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
		///ֹ���
		///inputOrderField    StopPrice;
		///ǿƽԭ��: ��ǿƽ
		inputOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///�Զ������־: ��
		inputOrderField.IsAutoSuspend = 0;
		///�û�ǿ����־: ��
		inputOrderField.UserForceClose = 0;
		int i = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
		LOGGER::CTP_Log("�µ�����%d",i);

	}

	if (this->portfoliomanager->GetOrderCount() == 0 && this->portfoliomanager->GetPostionDetailCount() >0 && this->portfoliomanager->GetOrderInputCount() == 0) { //�޸ĳ� ����µ�����û��size �ȴ��ɽ��Ķ���Ҳû��
		LOGGER::CTP_Log("��ʼ�µ�");
		//if (pDepthMarketData->SettlementPrice <= pDepthMarketData->BidPrice1)
		//{


		//return;
		//}

		CThostFtdcInputOrderField inputOrderField;
		(void)memset(&inputOrderField, 0, sizeof(inputOrderField));
		strcpy(inputOrderField.BrokerID, this->brokerid.c_str());
		strcpy(inputOrderField.InvestorID, this->investid.c_str());
		strcpy(inputOrderField.InstrumentID, pDepthMarketData->InstrumentID);
		double pricetick = this->instrumentmap->getInstrumentid(pDepthMarketData->InstrumentID).PriceTick;
		sprintf(inputOrderField.OrderRef, "%s", this->GetNewOrderRef().c_str());
		///�����۸�����
		inputOrderField.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
		///��������
		inputOrderField.Direction = THOST_FTDC_D_Sell;
		///��Ͽ�ƽ��־
		inputOrderField.CombOffsetFlag[0] = THOST_FTDC_OFEN_Close;
		///���Ͷ���ױ���־
		inputOrderField.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
		inputOrderField.LimitPrice = pDepthMarketData->BidPrice1 - 1.0*pricetick;
		inputOrderField.VolumeTotalOriginal = 1;
		///��Ч������: ������Ч
		inputOrderField.TimeCondition = THOST_FTDC_TC_GFD;
		///�ɽ�������: �κ�����
		inputOrderField.VolumeCondition = THOST_FTDC_VC_AV;
		///��С�ɽ���: 1
		inputOrderField.MinVolume = 1;
		///��������: ����
		inputOrderField.ContingentCondition = THOST_FTDC_CC_Immediately;
		///ֹ���
		///inputOrderField    StopPrice;
		///ǿƽԭ��: ��ǿƽ
		inputOrderField.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		///�Զ������־: ��
		inputOrderField.IsAutoSuspend = 0;
		///�û�ǿ����־: ��
		inputOrderField.UserForceClose = 0;
		int i = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
		LOGGER::CTP_Log("�µ�����%d", i);

	}


}
void  StrategyCat::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {
	LOGGER::CTP_Log("�µ�ʧ�� %s",pRspInfo->ErrorMsg);
 }
void  StrategyCat::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	LOGGER::CTP_Log("�µ�����%s",pOrder->StatusMsg);
}