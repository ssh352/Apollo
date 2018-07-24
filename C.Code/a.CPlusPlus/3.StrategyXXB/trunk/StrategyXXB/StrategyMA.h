#pragma once
#include "AbstractStrategy.h"
#include "RecordData.h"
#include <time.h>
using namespace Abstract;
using namespace std;
/***均线趋势策略

***/
class StrategyMA : public AbstractStrategy
{
public:
	StrategyMA();
	~StrategyMA();
protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnTime(tm t);
	virtual void OnInit();

public:
	double EMA20;
	double EMA10;
	double EMATemp20;
	double EMATemp10;
	double Standard;
	int SellPos;
	int BuyPos;
	int SellTryWrongTime;
	int BuyTryWrongTime;
	int bSellWatch;
	int bBuyWatch;
	int bSellFar;
	int bBuyFar;
	int SellOpen;
	int BuyOpen;
	int DayCount1;			//不是远离的情况下的计数
	int DayCount2;			//远离情况下的计数
	int bMiss;					//是否错过趋势
	int bCross;
	int bFinish;
	int bDoneHalf;
	int SellCloseHalf;
	int BuyCloseHalf;
	int SellCloseAll;
	int BuyCloseAll;
	int bGlued;
	int bJudged;
	CString bSellWatchDate;
	CString bBuyWatchDate;
	CString bSellFarDate;
	CString bBuyFarDate;
	string BuyLastDate;
	string SellLastDate;
	string BuyLastTime;
	string SellLastTime;
	CThostFtdcDepthMarketDataField NowTick;

public:
	CThostFtdcInputOrderField MakeOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition);
	CThostFtdcInputOrderField MakeLockOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition);
	CThostFtdcInputOrderField MakeCloseOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition);
	time_t GetTimeStamp(string date, string time);
};