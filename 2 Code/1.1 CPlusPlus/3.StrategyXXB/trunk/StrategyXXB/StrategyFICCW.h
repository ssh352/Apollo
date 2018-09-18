#pragma once
#include "AbstractStrategy.h"
#include "LastTickMap.h""
using namespace Abstract;

class StrategyFICCW :public AbstractStrategy
{
public:
	StrategyFICCW();
	virtual ~StrategyFICCW();

protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade);
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
	virtual void OnRspQryOrder(CThostFtdcOrderField *pOrder, int nRequestID, bool bIsLast);
	virtual void OnRspQryTrade(CThostFtdcTradeField *pTrade, int nRequestID, bool bIsLast);
private:
	vector<string> GetPostionListFromFile(string files);
	bool IsNeedTradeLong();
	bool IsNeedTradeShort();
	bool IsInLongList(string instrumentid);  //�ǲ����ڶ࿪�б�
	bool IsInShortList(string instrumentid); //�ǲ����ڿտ��б�
	time_t StringToDatetime(string str);

	void TradeLong(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void TradeShort(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void PyRun();
private:
	bool needtradelong;//�Ƿ���Ҫ���׶�
	bool needtradeshort;//�Ƿ���Ҫ���׿�
	bool hascompletetradelong;//�Ƿ��Ѿ�������϶��
	bool hascompletetradeshort; //�Ƿ��Ѿ�������Ͽղ�
	char lastradelongday[9]; //���ִ�н��׵�����
	char lastradeshortday[9]; //���ִ�н��׵�����
	double singglemount; //��Ʒ�ֽ��׽��
	time_t lasttradingtime;
	time_t lastpythonruntime;
	vector<string> list_longpositon; //����
	vector<string> list_shortpositon;

};
