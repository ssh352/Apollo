#pragma once
#include "AbstractStrategy.h"
#include "RecordData.h"
#include <time.h>
using namespace Abstract;
using namespace std;
/***������40�վ�������***/
class StrategyMA40Alert : public AbstractStrategy
{
public:
	StrategyMA40Alert();
	~StrategyMA40Alert();
protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //�ɽ��ر�		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //������պ���
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnTime(tm t);
	virtual void OnInit();

public:
	map<CString, RecordData> m_RecordData;

public:
	int flag;
};
