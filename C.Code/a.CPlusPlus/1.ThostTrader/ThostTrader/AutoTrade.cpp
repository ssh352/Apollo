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

char  FRONT_ADDR_1A[] = "tcp://180.168.146.187:10000";		// 前置地址1交易:实盘
char  FRONT_ADDR_1B[] = "tcp://180.168.146.187:10010";		// 前置地址2行情:实盘

char  FRONT_ADDR_2A[] = "tcp://180.168.146.187:10001";		// 前置地址1交易:盘后
char  FRONT_ADDR_2B[] = "tcp://180.168.146.187:10011";		// 前置地址2行情:盘后

char  FRONT_ADDR_3A[] = "tcp://218.202.237.33 :10002";	// 前置地址3交易:仿真 17:00开始
char  FRONT_ADDR_3B[] = "tcp://218.202.237.33 :10012";		// 前置地址3行情:仿真 17:00开始

TThostFtdcBrokerIDType	BROKER_ID = "9999";								    // 实盘：经纪公司代码 国泰君安=7090
TThostFtdcInvestorIDType INVESTOR_ID = "111180";						    // 实盘：投资者代码
TThostFtdcPasswordType  PASSWORD = "hexin123";							// 实盘：用户密码
TThostFtdcInstrumentIDType INSTRUMENT_ID = "";					// 交易合约代码
TThostFtdcDirectionType	DIRECTION;										// 交易买卖方向
TThostFtdcOffsetFlagType MARKETState;									// 开平仓
TThostFtdcPriceType	LIMIT_PRICE;										        // 交易价格

char *ppInstrumentID[] = { "jm1809", "hc1809", "SM809", "rb1809"};									// 行情订阅列表
int iInstrumentID = 4;													// 行情订阅数量

bool	RunMode = 0;														// 实盘下单=1,否则=0；  影响Common.h中的SendOrder()函数
bool	ReceiveTick = false;

// 请求编号
int iRequestID = 0;
// 交易时间
bool	JustRun = false;	//正在启动标志

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

	// 初始化UserApi
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi("./thosttraderapi.dll");			// 创建UserApi//"./thosttraderapi.dll"
	CTraderSpi* pUserSpi = new CTraderSpi();
	pUserApi->RegisterSpi((CThostFtdcTraderSpi*)pUserSpi);			    // 注册事件类
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);				// 注册公有流
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);			    // 注册私有流
	pUserApi->RegisterFront(FRONT_ADDR_1A);							        // connect

	pUserApi->Init();

	// 初始化MdApi
	pMdApi = CThostFtdcMdApi::CreateFtdcMdApi("./thostmduserapi.dll");					// 创建MdApi//"./thostmduserapi.dll"
	CThostFtdcMdSpi* pMdSpi = new CMdSpi();
	pMdApi->RegisterSpi(pMdSpi);									            // 注册事件类
	pMdApi->RegisterFront(FRONT_ADDR_1B);							// connect		优先行情地址
	pMdApi->RegisterFront(FRONT_ADDR_2B);							// connect		备用行情地址，1B断开，自动连接2B地址

	pMdApi->Init();

	Sleep(6000);

	// ReadConfiguration("./AutoTrader.dat");			//自定义数据，如持仓数据等均可

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



