// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include ".\ThostTraderApi\ThostFtdcTraderApi.h"
#include "TraderSpi.h"

// UserApi����
CThostFtdcTraderApi* pUserApi;

// ���ò���
char  FRONT_ADDR[] = "tcp://asp-sim2-front1.financial-trading-platform.com:26205";		// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����
TThostFtdcInvestorIDType INVESTOR_ID = "354079";			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "481531";			// �û�����
TThostFtdcInstrumentIDType INSTRUMENT_ID = "cu1408";	// ��Լ����
TThostFtdcDirectionType	DIRECTION = THOST_FTDC_D_Sell;	// ��������
TThostFtdcPriceType	LIMIT_PRICE = 60000; // 38850;				// �۸�

// ������
int iRequestID = 0;

void main(void)
{
	// ��ʼ��UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// ����UserApi
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_QUICK);				// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_QUICK);				// ע��˽����
	pUserApi->RegisterFront(FRONT_ADDR);							// connect
	pUserApi->Init();

	pUserApi->Join();
//	pUserApi->Release();
}