#pragma once
#include "AbstractStrategy.h"
#include "KBarGenerater1.h"
using namespace Abstract;
class StrategyCat:public AbstractStrategy
{
public:
	StrategyCat();
	virtual ~StrategyCat();
private :
	int  postion;//�ҵ��ڶ����е�λ��
	KBarGenerater1 * kbargen;
	string symbol;

protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) ; //�ɽ��ر�		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) ; //������պ���
	virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) ;
	virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) ;
	virtual void OnInit();
	void OnBar(TKDATA kdata);
	virtual void OnTime(tm t);
};
