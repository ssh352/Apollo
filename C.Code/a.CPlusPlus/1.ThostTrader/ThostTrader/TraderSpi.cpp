#include <iostream>
#include <fstream>
#include <cstring>
#include <windows.h>
using namespace std;

#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"

#pragma warning(disable : 4996)

// USER_API����
extern CThostFtdcTraderApi* pUserApi;

// ���ò���
extern char FRONT_ADDR[];		// ǰ�õ�ַ
extern char BROKER_ID[];		// ���͹�˾����
extern char INVESTOR_ID[];		// Ͷ���ߴ���
extern char PASSWORD[];			// �û�����
extern char* ppInstrumentID[];
extern int iInstrumentID;
extern char INSTRUMENT_ID[];	// ��Լ����
extern TThostFtdcDirectionType	DIRECTION;	// ��������
extern TThostFtdcOffsetFlagType MARKETState;//��ƽ��
extern TThostFtdcPriceType	LIMIT_PRICE;	// �۸�

											// ������
extern int iRequestID;
extern	TThostFtdcDateExprType	TradingDay;

extern	double	Q_UpperLimit;	//
extern	double	Q_LowerLimit;	//

extern	bool	JustRun;		//����������־

								// �Ự����
TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
TThostFtdcOrderRefType	ORDER_REF;	//��������


void CTraderSpi::OnFrontConnected()
{
	Sleep(3000);
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> " << "CTraderSpi::Delay 3s!" << endl;

	///�û���¼����
	CTraderSpi::ReqUserLogin();
}

void CTraderSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> ���ͽ����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
	CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;

	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		//cerr << "--->>> �������� = " << ORDER_REF << endl;
		///��ȡ��ǰ������
		strcpy(TradingDay, pUserApi->GetTradingDay());
		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		///Ͷ���߽�����ȷ��
		ReqSettlementInfoConfirm();
	}
}

void CTraderSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	cerr << "--->>> Ͷ���߽�����ȷ��: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		std::cout << "=====Ͷ���߽�����ȷ�ϳɹ�=====" << std::endl;
		std::cout << "ȷ�����ڣ� " << pSettlementInfoConfirm->ConfirmDate << std::endl;
		std::cout << "ȷ��ʱ�䣺 " << pSettlementInfoConfirm->ConfirmTime << std::endl;
		///�����ѯ��Լ
		ReqQryInstrument();
	}
}


void CTraderSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
	cerr << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspQryInstrument(CThostFtdcInstrumentField *pInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//cerr << "--->>> " << __FUNCTION__ << endl;
	//if (bIsLast && !IsErrorRspInfo(pRspInfo))
	//{
	//	std::cout << "=====��ѯ��Լ����ɹ�=====" << std::endl;
	//	std::cout << "���������룺 " << pInstrument->ExchangeID << std::endl;
	//	std::cout << "��Լ���룺 " << pInstrument->InstrumentID << std::endl;
	//	std::cout << "��Լ�ڽ������Ĵ��룺 " << pInstrument->ExchangeInstID << std::endl;
	//	std::cout << "ִ�мۣ� " << pInstrument->StrikePrice << std::endl;
	//	std::cout << "�����գ� " << pInstrument->EndDelivDate << std::endl;
	//	std::cout << "��ǰ����״̬�� " << pInstrument->IsTrading << std::endl;
	//}

	///�����ѯ��Լ
	ReqQryTradingAccount();
}

void CTraderSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
	cerr << "--->>> �����ѯ�ʽ��˻�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField *pTradingAccount, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		std::cout << "=====��ѯͶ�����ʽ��˻��ɹ�=====" << std::endl;
		std::cout << "Ͷ�����˺ţ� " << pTradingAccount->AccountID << std::endl;
		std::cout << "�����ʽ� " << pTradingAccount->Available << std::endl;
		std::cout << "��ȡ�ʽ� " << pTradingAccount->WithdrawQuota << std::endl;
		std::cout << "��ǰ��֤��: " << pTradingAccount->CurrMargin << std::endl;
		std::cout << "ƽ��ӯ���� " << pTradingAccount->CloseProfit << std::endl;

		///�����ѯͶ���ֲ߳�		
		ReqQryInvestorPosition();
	}
}

void CTraderSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
	cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}


void CTraderSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		std::cout << "=====��ѯͶ���ֲֳ߳ɹ�=====" << std::endl;
		if (pInvestorPosition)
		{
			std::cout << "��Լ���룺 " << pInvestorPosition->InstrumentID << std::endl;
			std::cout << "���ּ۸� " << pInvestorPosition->OpenAmount << std::endl;
			std::cout << "�������� " << pInvestorPosition->OpenVolume << std::endl;
			std::cout << "���ַ��� " << pInvestorPosition->PosiDirection << std::endl;
			std::cout << "ռ�ñ�֤��" << pInvestorPosition->UseMargin << std::endl;
		}
		else
			std::cout << "----->�ú�Լδ�ֲ�" << std::endl;

		///����¼������
		//ReqOrderInsert();                //--����״̬�����µ�
		ReqQryInvestorPositionDetail();

	}

}

void CTraderSpi::ReqQryInvestorPositionDetail()
{
	CThostFtdcQryInvestorPositionDetailField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.InvestorID, INVESTOR_ID);
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	int iResult = pUserApi->ReqQryInvestorPositionDetail(&req, ++iRequestID);
	cerr << "--->>> �����ѯͶ���ֲ߳���ϸ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspQryInvestorPositionDetail(CThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		std::cout << "=====��ѯͶ���ֲ߳���ϸ�ɹ�=====" << std::endl;
		if (pInvestorPositionDetail)
		{
			std::cout << "��Լ���룺 " << pInvestorPositionDetail->InstrumentID << std::endl;
			std::cout << "���ּ۸� " << pInvestorPositionDetail->OpenPrice << std::endl;
			std::cout << "�������� " << pInvestorPositionDetail->Volume << std::endl;
			std::cout << "���ַ��� " << pInvestorPositionDetail->Direction << std::endl;
		}
		else
			std::cout << "----->�ú�Լδ�ֲ�" << std::endl;

	}

}

void CTraderSpi::ReqOrderInsert()
{
	//const char bsstat;
	//char bsopenstat;

	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, INVESTOR_ID);
	///��Լ����
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	///��������
	strcpy(req.OrderRef, ORDER_REF);
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;
	///��������: 
	req.Direction = DIRECTION;
	///��Ͽ�ƽ��־: ����
	req.CombOffsetFlag[0] = MARKETState;
	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = LIMIT_PRICE;
	///����: 1
	req.VolumeTotalOriginal = 1;
	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
	//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 1;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	req.IsAutoSuspend = 0;
	///ҵ��Ԫ
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
	req.UserForceClose = 0;

	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
	cerr << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void CTraderSpi::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;

	SYSTEMTIME sys_time;
	GetLocalTime(&sys_time);
	double system_times;
	system_times = (double)((sys_time.wHour) / 10e1) + (double)((sys_time.wMinute) / 10e3) + (double)((sys_time.wSecond) / 10e5);	//��ʽʱ��0.145100

	cerr << "--->>> ����: " << TradingDay << "_" << system_times << "_" << INSTRUMENT_ID << "_" << ((DIRECTION == '0') ? "��" : "��") << "_" << ((MARKETState == '0') ? "����" : "ƽ��") << "_" << LIMIT_PRICE << endl;
	IsErrorRspInfo(pRspInfo);
}

void CTraderSpi::ReqOrderAction(CThostFtdcOrderField *pOrder)
{
	static bool ORDER_ACTION_SENT = false;		//�Ƿ����˱���
	if (ORDER_ACTION_SENT)
		return;

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, pOrder->BrokerID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, pOrder->InvestorID);
	///������������
	//	TThostFtdcOrderActionRefType	OrderActionRef;
	///��������
	strcpy(req.OrderRef, pOrder->OrderRef);
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///ǰ�ñ��
	req.FrontID = FRONT_ID;
	///�Ự���
	req.SessionID = SESSION_ID;
	///����������
	//	TThostFtdcExchangeIDType	ExchangeID;
	///�������
	//	TThostFtdcOrderSysIDType	OrderSysID;
	///������־
	req.ActionFlag = THOST_FTDC_AF_Delete;
	///�۸�
	//	TThostFtdcPriceType	LimitPrice;
	///�����仯
	//	TThostFtdcVolumeType	VolumeChange;
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///��Լ����
	strcpy(req.InstrumentID, pOrder->InstrumentID);

	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
	cerr << "--->>> ������������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
	ORDER_ACTION_SENT = true;
}

void CTraderSpi::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);

}

///����֪ͨ
void CTraderSpi::OnRtnOrder(CThostFtdcOrderField *pOrder)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	
	if (IsMyOrder(pOrder))
	{
		if (IsTradingOrder(pOrder))
			ReqOrderAction(pOrder);
		else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
			cout << "--->>> �����ɹ�" << endl;
	}
}

///�ɽ�֪ͨ
void CTraderSpi::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
	cerr << "--->>> " << "OnRtnTrade" << endl;

	std::cout << "=====�����ɹ��ɽ�=====" << std::endl;
	std::cout << "�ɽ�ʱ�䣺 " << pTrade->TradeTime << std::endl;
	std::cout << "��Լ���룺 " << pTrade->InstrumentID << std::endl;
	std::cout << "�ɽ��۸� " << pTrade->Price << std::endl;
	std::cout << "�ɽ����� " << pTrade->Volume << std::endl;
	std::cout << "��ƽ�ַ��� " << pTrade->Direction << std::endl;

}

void CTraderSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}

void CTraderSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CTraderSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool CTraderSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

bool CTraderSpi::IsMyOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
		(pOrder->SessionID == SESSION_ID) &&
		(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool CTraderSpi::IsTradingOrder(CThostFtdcOrderField *pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

