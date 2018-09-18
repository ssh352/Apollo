#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include <time.h>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;

#include "ThostFtdcTraderApi.h"
#include "ThostFtdcMdApi.h"
#include "TraderSpi.h"
#include "MdSpi.h"
#include "Common.h"
#include "DataSniffer.h"
#include "MyTrader.h"

#pragma warning(disable : 4996)

CThostFtdcTraderApi *pUserApi;
CThostFtdcMdApi *pMdApi;

char  FRONT_ADDR_1A[] = "tcp://180.168.146.187:10000";		// ǰ�õ�ַ1����:ʵ��
char  FRONT_ADDR_1B[] = "tcp://180.168.146.187:10010";		// ǰ�õ�ַ2����:ʵ��

char  FRONT_ADDR_2A[] = "tcp://180.168.146.187:10001";		// ǰ�õ�ַ1����:�̺�
char  FRONT_ADDR_2B[] = "tcp://180.168.146.187:10011";		// ǰ�õ�ַ2����:�̺�

char  FRONT_ADDR_3A[] = "tcp://218.202.237.33 :10002";	// ǰ�õ�ַ3����:���� 17:00��ʼ
char  FRONT_ADDR_3B[] = "tcp://218.202.237.33 :10012";		// ǰ�õ�ַ3����:���� 17:00��ʼ

TThostFtdcBrokerIDType	BROKER_ID = "9999";								    // ʵ�̣����͹�˾���� ��̩����=7090
TThostFtdcInvestorIDType INVESTOR_ID = "111180";						    // ʵ�̣�Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "hexin123";							// ʵ�̣��û�����
TThostFtdcInstrumentIDType INSTRUMENT_ID = "";					// ���׺�Լ����
TThostFtdcDirectionType	DIRECTION;										// ������������
TThostFtdcOffsetFlagType MARKETState;									// ��ƽ��
TThostFtdcPriceType	LIMIT_PRICE;										        // ���׼۸�

char *ppInstrumentID[] = { "jm1809", "hc1809", "SM809", "rb1809"};									// ���鶩���б�
int iInstrumentID = 4;													// ���鶩������

bool	RunMode = 0;														// ʵ���µ�=1,����=0��  Ӱ��Common.h�е�SendOrder()����
bool	ReceiveTick = false;

// ������
int iRequestID = 0;
// ����ʱ��
bool	JustRun = false;	//����������־

TThostFtdcDateExprType	TradingDay;

void main(void)
{
	vector<string> long_instrument;

	void Erasefiles();
	void Sniffer();
	void Trading(char *instrument);
	bool ReadConfiguration(char *filepaths);
	void WriteConfiguration(char *filepaths);

	Erasefiles();
	Sleep(2000);

	cerr << "--->>> " << "Welcom MyAutoTrader System!" << endl;

	// ��ʼ��UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./thosttraderapi.dll");			// ����UserApi//"./thosttraderapi.dll"
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			    // ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			    // ע��˽����
	pUserApi->RegisterFront(FRONT_ADDR_1A);							        // connect

	pUserApi->Init();

	// ��ʼ��MdApi
	pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll");					// ����MdApi//"./thostmduserapi.dll"
	CThostFtdcMdSpi* pMdSpi = new CMdSpi();
	pMdApi->RegisterSpi(pMdSpi);									            // ע���¼���
	pMdApi->RegisterFront(FRONT_ADDR_1B);							// connect		���������ַ
	pMdApi->RegisterFront(FRONT_ADDR_2B);							// connect		���������ַ��1B�Ͽ����Զ�����2B��ַ

	pMdApi->Init();

	Sleep(6000);

	// ReadConfiguration("./AutoTrader.dat");			//�Զ������ݣ���ֲ����ݵȾ���

	long_instrument = GetInstrumentList("long.csv");

	for (int i = 1; i < long_instrument.size(); i++)
	{
		strcpy(INSTRUMENT_ID, long_instrument[i].c_str());
		Sniffer();

		Trading(INSTRUMENT_ID);

		Sleep(5000);
	}

	pMdApi->Join();
	delete pMdSpi;
	pMdApi->Release();

	pUserApi->Join();
	delete pUserSpi;
	pUserApi->Release();

}



