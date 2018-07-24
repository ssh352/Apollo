#include "stdafx.h"
#include "StrategyFlirt.h"
#include "RecordData.h"
#include "mmsystem.h"
#include <list>
#include <map>
#include "LOGGER.h"
#pragma comment(lib,"winmm.lib")

//#include "InStrumentMap.h"
using namespace std;
using namespace CTPTOOL;


int GetSections(CStringArray& arrSection)
{
	/*
	������������
	GetPrivateProfileSectionNames - �� ini �ļ��л�� Section ������
	��� ini �������� Section: [sec1] �� [sec2]���򷵻ص��� 'sec1',0,'sec2',0,0 �����㲻֪��
	ini ������Щ section ��ʱ���������� api ����ȡ����
	*/
	int i;
	int iPos = 0;
	int iMaxCount;
    #define MAX_ALLSECTIONS 1024
	TCHAR chSectionNames[MAX_ALLSECTIONS] = { 0 }; //�ܵ���������ַ���
	TCHAR chSection[MAX_ALLSECTIONS] = { 0 }; //���һ��������
	GetPrivateProfileSectionNames(chSectionNames, MAX_ALLSECTIONS, ".\\OpenPer.ini");

	//����ѭ�����ضϵ�����������0
	for (i = 0; i<MAX_ALLSECTIONS; i++)
	{
		if (chSectionNames[i] == 0)
			if (chSectionNames[i] == chSectionNames[i + 1])
				break;
	}

	iMaxCount = i + 1; //Ҫ��һ��0��Ԫ�ء����ҳ�ȫ���ַ����Ľ������֡�
					   //arrSection.RemoveAll();//���ԭ����

	for (i = 0; i<iMaxCount; i++)
	{
		chSection[iPos++] = chSectionNames[i];
		if (chSectionNames[i] == 0)
		{
			arrSection.Add(chSection);
			memset(chSection, 0, i);
			iPos = 0;
		}

	}

	return (int)arrSection.GetSize();
}
StrategyFlirt::StrategyFlirt()  
{
	CStringArray subscribe;
	GetSections(subscribe);
	for (int i = 0; i < subscribe.GetSize(); i++) {
		if (subscribe[i] != "End")
		{
			this->subscribelist.push_back(subscribe[i].GetBuffer());
		}
		

	}
	/*
	this->subscribelist.push_back("m1801");
	this->subscribelist.push_back("y1709");
	this->subscribelist.push_back("y1801");
	this->subscribelist.push_back("p1709");
	this->subscribelist.push_back("p1801");
	this->subscribelist.push_back("i1709");
	this->subscribelist.push_back("i1801");
	this->subscribelist.push_back("l1709");
	this->subscribelist.push_back("l1801");
	this->subscribelist.push_back("jd1705");
	this->subscribelist.push_back("jd1801");
	this->subscribelist.push_back("v1709");
	this->subscribelist.push_back("v1801");
	this->subscribelist.push_back("pp1709");
	this->subscribelist.push_back("pp1801");
	this->subscribelist.push_back("c1709");
	this->subscribelist.push_back("c1801");
	this->subscribelist.push_back("a1709");
	this->subscribelist.push_back("a1801");
	this->subscribelist.push_back("cs1709");
	this->subscribelist.push_back("cs1801");
	this->subscribelist.push_back("SR709");
	this->subscribelist.push_back("SR801");
	this->subscribelist.push_back("TA709");
	this->subscribelist.push_back("TA801");
	this->subscribelist.push_back("OI709");
	this->subscribelist.push_back("OI801");
	this->subscribelist.push_back("CF709");
	this->subscribelist.push_back("CF801");
	this->subscribelist.push_back("MA709");
	this->subscribelist.push_back("MA801");
	this->subscribelist.push_back("FG709");
	this->subscribelist.push_back("FG801");
	this->subscribelist.push_back("RM709");
	this->subscribelist.push_back("RM801");
	this->subscribelist.push_back("cu1705");
	this->subscribelist.push_back("cu1706");
	this->subscribelist.push_back("zn1705");
	this->subscribelist.push_back("zn1706");
	this->subscribelist.push_back("ni1705");
	this->subscribelist.push_back("ru1705");
	this->subscribelist.push_back("ru1709");
	this->subscribelist.push_back("rb1705");
	this->subscribelist.push_back("rb1710");
	this->subscribelist.push_back("rb1801");
	this->subscribelist.push_back("hc1710");
	this->subscribelist.push_back("hc1801");
	this->subscribelist.push_back("bu1706");
	this->subscribelist.push_back("bu1709");
	this->subscribelist.push_back("al1705");
	this->subscribelist.push_back("al1706");
	this->subscribelist.push_back("jm1709");
	this->subscribelist.push_back("jm1801");
	this->subscribelist.push_back("j1709");
	this->subscribelist.push_back("j1801");
	this->subscribelist.push_back("ZC709");
	this->subscribelist.push_back("ZC801");
	this->subscribelist.push_back("pb1705");
	this->subscribelist.push_back("pb1706");
	this->subscribelist.push_back("au1706");
	this->subscribelist.push_back("au1712");
	this->subscribelist.push_back("ag1706");
	this->subscribelist.push_back("ag1712");
	this->subscribelist.push_back("T1706");
	this->subscribelist.push_back("T1709");
	this->subscribelist.push_back("TF1706");
	this->subscribelist.push_back("TF1709");
	this->subscribelist.push_back("IC1706");
	this->subscribelist.push_back("IC1704");
	this->subscribelist.push_back("IH1706");
	this->subscribelist.push_back("IH1704");
	this->subscribelist.push_back("IF1706");
	this->subscribelist.push_back("IF1704");
	*/
	this->strategyname = "�������";
	this->strategyid = 3001;
}


StrategyFlirt::~StrategyFlirt(){

} 

void StrategyFlirt::OnRtnTrade(CThostFtdcTradeField *pTrade) {  

	//��ʾ���������
	sndPlaySound("����ɹ�.wav", SND_ASYNC);
}

void StrategyFlirt::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

void StrategyFlirt::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {

	//CThostFtdcInstrumentField instrument = this->instrumentmap.get()->getInstrumentid(pDepthMarketData->InstrumentID);
	CString StrUpdateTime = pDepthMarketData->UpdateTime;
	CString StrOpenPer;
	GetPrivateProfileString(pDepthMarketData->InstrumentID, "OpenPer", "NoValue", StrOpenPer.GetBuffer(7), 7, ".\\OpenPer.ini");
	StrOpenPer.ReleaseBuffer();
	double OpenPer = atof(StrOpenPer.GetBuffer(0));
	StrOpenPer.ReleaseBuffer();
	GetPrivateProfileString(pDepthMarketData->InstrumentID, "WarningPer", "NoValue", StrOpenPer.GetBuffer(7), 7, ".\\OpenPer.ini");
	StrOpenPer.ReleaseBuffer();
	double WarningPer = atof(StrOpenPer.GetBuffer(0));

	//��ȡ�����ļ��еĿ���ʱ��
	//21����Ϊ1��9����Ϊ2��
	GetPrivateProfileString(pDepthMarketData->InstrumentID, "OpenTime", "NoValue", StrOpenPer.GetBuffer(7), 7, ".\\OpenPer.ini");
	StrOpenPer.ReleaseBuffer();
	int OpenTimeMark = atoi(StrOpenPer.GetBuffer(0));

	//9�㿪�̵ĺ�Լ
	if (OpenTimeMark == 2) {
		if (StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("01") == 0
			&& (abs((pDepthMarketData->LastPrice - pDepthMarketData->PreSettlementPrice) / pDepthMarketData->PreSettlementPrice) < OpenPer) == true) {

			//IH ,IC,IF TF,T ����ȥ����
			if ((pDepthMarketData->InstrumentID[0] == 'I' && (pDepthMarketData->InstrumentID[1] == 'C' || pDepthMarketData->InstrumentID[1] == 'F' || pDepthMarketData->InstrumentID[1] == 'H')) || (pDepthMarketData->InstrumentID[0] == 'T' && (pDepthMarketData->InstrumentID[1] == '1' || pDepthMarketData->InstrumentID[1] == 'F')))
			{
				return;
			}

			if (this->portfoliomanager->GetPostionDetailCount() == 0 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open) == 0) {
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
				inputOrderField1.LimitPrice = (pDepthMarketData->PreSettlementPrice*(1 + WarningPer));
				CThostFtdcInstrumentField instrument = this->instrumentmap.get()->getInstrumentid(inputOrderField1.InstrumentID);
				double pricetick = instrument.PriceTick;
				inputOrderField1.LimitPrice = floor(inputOrderField1.LimitPrice / pricetick)*pricetick;
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

			//�µ�ͣ���򿪶���
			if (this->portfoliomanager->GetPostionDetailCount() == 0 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open) == 0) {
				CThostFtdcInputOrderField inputOrderField2;
				memset(&inputOrderField2, 0, sizeof(inputOrderField2));
				strcpy(inputOrderField2.BrokerID, this->brokerid.c_str());
				strcpy(inputOrderField2.InvestorID, this->investid.c_str());
				strcpy(inputOrderField2.InstrumentID, pDepthMarketData->InstrumentID);
				sprintf(inputOrderField2.OrderRef, "%s", this->GetNewOrderRef().c_str());
				///�����۸�����
				inputOrderField2.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				///��������
				inputOrderField2.Direction = THOST_FTDC_D_Buy;

				///��Ͽ�ƽ��־
				inputOrderField2.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;
				///���Ͷ���ױ���־
				inputOrderField2.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
				CThostFtdcInstrumentField instrument = this->instrumentmap.get()->getInstrumentid(inputOrderField2.InstrumentID);
				double pricetick = instrument.PriceTick;
				inputOrderField2.LimitPrice = (pDepthMarketData->PreSettlementPrice*(1 - WarningPer));;
				//����ʹ��price pricetick��������
				inputOrderField2.LimitPrice = floor(inputOrderField2.LimitPrice / pricetick)*pricetick;

				inputOrderField2.VolumeTotalOriginal = 1;
				///��Ч������: ������Ч
				inputOrderField2.TimeCondition = THOST_FTDC_TC_GFD;
				///�ɽ�������: �κ�����
				inputOrderField2.VolumeCondition = THOST_FTDC_VC_AV;
				///��С�ɽ���: 1
				inputOrderField2.MinVolume = 1;
				///��������: ����
				inputOrderField2.ContingentCondition = THOST_FTDC_CC_Immediately;
				///ֹ���
				///inputOrderField    StopPrice;
				///ǿƽԭ��: ��ǿƽ
				inputOrderField2.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
				///�Զ������־: ��
				inputOrderField2.IsAutoSuspend = 0;

				///�û�ǿ����־: ��
				inputOrderField2.UserForceClose = 0;
				int j = this->tdapi->ReqOrderInsert(&inputOrderField2, this->requestid++);

			}

		}
	}
	//21�㿪�̵ĺ�Լ
	if (OpenTimeMark == 1) {
		if (StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("01") == 0                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			&& (abs((pDepthMarketData->LastPrice - pDepthMarketData->PreSettlementPrice) / pDepthMarketData->PreSettlementPrice) < OpenPer) == true) {

			//IH ,IC,IF TF,T ����ȥ����
			if ((pDepthMarketData->InstrumentID[0] == 'I' && (pDepthMarketData->InstrumentID[1] == 'C' || pDepthMarketData->InstrumentID[1] == 'F' || pDepthMarketData->InstrumentID[1] == 'H')) || (pDepthMarketData->InstrumentID[0] == 'T' && (pDepthMarketData->InstrumentID[1] == '1' || pDepthMarketData->InstrumentID[1] == 'F')))
			{
				return;
			}

			if (this->portfoliomanager->GetPostionDetailCount() == 0 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open) == 0) {
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
				inputOrderField1.LimitPrice = (pDepthMarketData->PreSettlementPrice*(1 + WarningPer));
				CThostFtdcInstrumentField instrument = this->instrumentmap.get()->getInstrumentid(inputOrderField1.InstrumentID);
				double pricetick = instrument.PriceTick;
				inputOrderField1.LimitPrice = floor(inputOrderField1.LimitPrice / pricetick)*pricetick;
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

			//�µ�ͣ���򿪶���
			if (this->portfoliomanager->GetPostionDetailCount() == 0 && this->portfoliomanager->GetOrderInputCount() == 0 && this->portfoliomanager->GetOrderCount(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open) == 0) {
				CThostFtdcInputOrderField inputOrderField2;
				memset(&inputOrderField2, 0, sizeof(inputOrderField2));
				strcpy(inputOrderField2.BrokerID, this->brokerid.c_str());
				strcpy(inputOrderField2.InvestorID, this->investid.c_str());
				strcpy(inputOrderField2.InstrumentID, pDepthMarketData->InstrumentID);
				sprintf(inputOrderField2.OrderRef, "%s", this->GetNewOrderRef().c_str());
				///�����۸�����
				inputOrderField2.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
				///��������
				inputOrderField2.Direction = THOST_FTDC_D_Buy;

				///��Ͽ�ƽ��־
				inputOrderField2.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;
				///���Ͷ���ױ���־
				inputOrderField2.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
				CThostFtdcInstrumentField instrument = this->instrumentmap.get()->getInstrumentid(inputOrderField2.InstrumentID);
				double pricetick = instrument.PriceTick;
				inputOrderField2.LimitPrice = (pDepthMarketData->PreSettlementPrice*(1 - WarningPer));;
				//����ʹ��price pricetick��������
				inputOrderField2.LimitPrice = floor(inputOrderField2.LimitPrice / pricetick)*pricetick;

				inputOrderField2.VolumeTotalOriginal = 1;
				///��Ч������: ������Ч
				inputOrderField2.TimeCondition = THOST_FTDC_TC_GFD;
				///�ɽ�������: �κ�����
				inputOrderField2.VolumeCondition = THOST_FTDC_VC_AV;
				///��С�ɽ���: 1
				inputOrderField2.MinVolume = 1;
				///��������: ����
				inputOrderField2.ContingentCondition = THOST_FTDC_CC_Immediately;
				///ֹ���
				///inputOrderField    StopPrice;
				///ǿƽԭ��: ��ǿƽ
				inputOrderField2.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
				///�Զ������־: ��
				inputOrderField2.IsAutoSuspend = 0;

				///�û�ǿ����־: ��
				inputOrderField2.UserForceClose = 0;
				int j = this->tdapi->ReqOrderInsert(&inputOrderField2, this->requestid++);

			}

		}
	}


	//����
	if (StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("06") == 0 || StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("06") == 0) {
		//IH ,IC,IF TF,T ����ȥ����
		if ((pDepthMarketData->InstrumentID[0] == 'I' &&( pDepthMarketData->InstrumentID[1]=='C'|| pDepthMarketData->InstrumentID[1] == 'F'|| pDepthMarketData->InstrumentID[1] == 'H')) || (pDepthMarketData->InstrumentID[0] == 'T' && (pDepthMarketData->InstrumentID[1] == '1' || pDepthMarketData->InstrumentID[1] == 'F' )))
		{
			return;
		}
		vector<CThostFtdcOrderField> list = this->portfoliomanager->GetOrder(pDepthMarketData->InstrumentID, THOST_FTDC_D_Buy, THOST_FTDC_OF_Open);
		for (int i = 0; i < list.size(); i++) {
			CThostFtdcInputOrderActionField inputOrderActionField1;
			(void)memset(&inputOrderActionField1, 0, sizeof(inputOrderActionField1));
			strcpy(inputOrderActionField1.BrokerID, this->brokerid.c_str());
			strcpy(inputOrderActionField1.InvestorID, this->investid.c_str());
			strcpy(inputOrderActionField1.OrderRef, list[i].OrderRef);
			inputOrderActionField1.FrontID = list[i].FrontID;
			inputOrderActionField1.SessionID = list[i].SessionID;
			inputOrderActionField1.ActionFlag = THOST_FTDC_AF_Delete;
			strcpy(inputOrderActionField1.InstrumentID, list[i].InstrumentID);
			this->tdapi->ReqOrderAction(&inputOrderActionField1, this->requestid++);

		}
		vector<CThostFtdcOrderField> list1 = this->portfoliomanager->GetOrder(pDepthMarketData->InstrumentID, THOST_FTDC_D_Sell, THOST_FTDC_OF_Open);
		for (int i = 0; i < list1.size(); i++) {
			CThostFtdcInputOrderActionField inputOrderActionField2;
			(void)memset(&inputOrderActionField2, 0, sizeof(inputOrderActionField2));
			strcpy(inputOrderActionField2.BrokerID, this->brokerid.c_str());
			strcpy(inputOrderActionField2.InvestorID, this->investid.c_str());
			strcpy(inputOrderActionField2.OrderRef, list1[i].OrderRef);
			inputOrderActionField2.FrontID = list1[i].FrontID;
			inputOrderActionField2.SessionID = list1[i].SessionID;
			inputOrderActionField2.ActionFlag = THOST_FTDC_AF_Delete;
			strcpy(inputOrderActionField2.InstrumentID, list1[i].InstrumentID);
			this->tdapi->ReqOrderAction(&inputOrderActionField2, this->requestid++);

		}



	}

	//���
	if ((StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("00") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("01") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("02") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("03") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("04") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("05") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("09") == 0 && StrUpdateTime.Mid(3, 2).Compare("06") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("00") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("01") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("02") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("02") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("03") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("04") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("05") == 0 ||
		StrUpdateTime.Mid(0, 2).Compare("21") == 0 && StrUpdateTime.Mid(3, 2).Compare("06") == 0) == 0) {

		list<double>::iterator TickIterator;
		m_RecordData[pDepthMarketData->InstrumentID].Tick10.push_back(pDepthMarketData->LastPrice);
		m_RecordData[pDepthMarketData->InstrumentID].Avr10 = 0.0;
		m_RecordData[pDepthMarketData->InstrumentID].Sum = 0.0;
		for (TickIterator = m_RecordData[pDepthMarketData->InstrumentID].Tick10.begin(); TickIterator != m_RecordData[pDepthMarketData->InstrumentID].Tick10.end(); ++TickIterator) {
			m_RecordData[pDepthMarketData->InstrumentID].Sum += *TickIterator;
		}
		m_RecordData[pDepthMarketData->InstrumentID].Avr10 = (m_RecordData[pDepthMarketData->InstrumentID].Sum- pDepthMarketData->LastPrice) / (m_RecordData[pDepthMarketData->InstrumentID].Tick10.size()-1);

		if (m_RecordData[pDepthMarketData->InstrumentID].Tick10.size() == 10) {
			m_RecordData[pDepthMarketData->InstrumentID].Tick10.pop_front();
		}
		if (abs((m_RecordData[pDepthMarketData->InstrumentID].Avr10 - pDepthMarketData->PreSettlementPrice)/ pDepthMarketData->PreSettlementPrice)< OpenPer &&
			abs((pDepthMarketData->LastPrice - pDepthMarketData->PreSettlementPrice)/ pDepthMarketData->PreSettlementPrice> WarningPer)) {
			char str_instrumentid[1024];
			memset(str_instrumentid, 0, sizeof(str_instrumentid));
			memcpy(str_instrumentid, pDepthMarketData->InstrumentID, sizeof(TThostFtdcInstrumentIDType));
			LOG("��������Ϣ��ʾ%s", str_instrumentid);
			sndPlaySound("��ʾ�µ�.wav", SND_ASYNC);
		}
	}
}


void StrategyFlirt::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

void StrategyFlirt::OnRtnOrder(CThostFtdcOrderField *pOrder) {

}

void StrategyFlirt::OnInit() {
	
}
void StrategyFlirt::OnTime(tm t)
{

}