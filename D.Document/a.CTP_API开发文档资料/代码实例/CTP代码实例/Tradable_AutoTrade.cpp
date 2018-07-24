#include <iostream>
#include <Windows.h>
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "ThostFtdcUserApiDataType.h"
#include "ThostFtdcUserApiStruct.h"

using namespace std;

HANDLE g_hEvent = CreateEvent(NULL, true, false, NULL);
TThostFtdcBrokerIDType g_chBrokerID;
TThostFtdcUserIDType g_chUserID;

class CSimpleHandler : public CThostFtdcTraderSpi
{
public:
	CSimpleHandler(CThostFtdcTraderApi *pUserApi)
		:m_pUserApi(pUserApi) {};
	~CSimpleHandler() {};

	virtual void OnFrontConnected()
	{
		CThostFtdcReqUserLoginField reqUserLogin;
		printf("BrokerID:\n");
		//scanf_s("%s", &g_chBrokerID, 16);
		strcpy_s(g_chBrokerID, "9999");
		strcpy_s(reqUserLogin.BrokerID, g_chBrokerID);

		printf("userid:");
		//scanf_s("%s", &g_chUserID, 16);
		strcpy_s(g_chUserID, "111180");
		strcpy_s(reqUserLogin.UserID, g_chUserID);

		printf("password:");
		//scanf_s("%s", &reqUserLogin.Password, 16);
		strcpy_s(reqUserLogin.Password, "hexin123");

		m_pUserApi->ReqUserLogin(&reqUserLogin, 0);
	}

	virtual void OnFrontDisconnected(int nReason)
	{
		printf("OnFrontDisconnected.\n");
	}

	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField*pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID,bool bIsLast)
	{
		printf("OnRspUserLogin:\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n", pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID, bIsLast);

		if (pRspInfo->ErrorID != 0)
		{
			printf("Failed to login, errorcode=%d errormsg=%s requestid = %d chain = %d",
				pRspInfo->ErrorID, pRspInfo->ErrorMsg, nRequestID, bIsLast);
			exit(-1);
		}

		CThostFtdcInputOrderField ord;
		memset(&ord, 0, sizeof(ord));
		strcpy_s(ord.BrokerID, g_chBrokerID);
		strcpy_s(ord.InvestorID, "111180");
		strcpy_s(ord.InstrumentID, "rb1805");
		strcpy_s(ord.OrderRef, "000000000001");
		strcpy_s(ord.UserID, g_chUserID);
		ord.OrderPriceType = THOST_FTDC_OPT_LimitPrice;  //1.�޼۵�
		//ord.OrderPriceType = THOST_FTDC_OPT_AnyPrice;   //1.SHFE��֧���м۵�
		ord.Direction = THOST_FTDC_D_Buy;
		strcpy_s(ord.CombOffsetFlag, "0");
		strcpy_s(ord.CombHedgeFlag, "1");
		ord.LimitPrice = 4050;  //�ɽ�����Ҫ����ʵ�ʵ���
		ord.VolumeTotalOriginal = 1;
		ord.TimeCondition = THOST_FTDC_TC_GFD;    //2.�޼۵�
		//ord.TimeCondition = THOST_FTDC_TC_IOC;    //2.SHFE��֧���м۵�
		strcpy_s(ord.GTDDate, "");
		ord.VolumeCondition = THOST_FTDC_VC_AV;
		ord.MinVolume = 0;
		ord.ContingentCondition = THOST_FTDC_CC_Immediately;
		ord.StopPrice = 0;
		ord.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
		ord.IsAutoSuspend = 0;

		m_pUserApi->ReqOrderInsert(&ord, 1);
	}

	virtual void OnRspOrderInsert(CThostFtdcInputOrderField*pInputOrder,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
	{
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n",pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		SetEvent(g_hEvent);
	};

	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder)
	{
		printf("OnRtnOrder:\n");
		printf("OrderSysID=[%s]\n", pOrder->OrderSysID);
	}

	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo,int nRequestID, bool bIsLast)
	{
		printf("OnRspError:\n");
		printf("ErrorCode=[%d], ErrorMsg=[%s]\n",pRspInfo->ErrorID, pRspInfo->ErrorMsg);
		printf("RequestID=[%d], Chain=[%d]\n", nRequestID,bIsLast);
		// the client should handle the error {error handle code}
	}

private:
	CThostFtdcTraderApi *m_pUserApi;
};

int main()
{
	CThostFtdcTraderApi *pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();
	printf("%s  %s", __TIME__, "CreateFtdcTraderApi Done!\n");
	CSimpleHandler sh(pUserApi);
	pUserApi->RegisterSpi(&sh);

	pUserApi->SubscribePrivateTopic(THOST_TERT_RESUME);
	pUserApi->SubscribePublicTopic(THOST_TERT_RESUME);

	// website: http://www.simnow.com.cn/product.action
	pUserApi->RegisterFront("tcp://180.168.146.187:10000");
	pUserApi->Init();
	WaitForSingleObject(g_hEvent, INFINITE);
	pUserApi->Release();
	return 0;
}