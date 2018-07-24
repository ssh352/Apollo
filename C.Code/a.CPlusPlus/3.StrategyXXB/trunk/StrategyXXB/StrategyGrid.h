#pragma once
#include "AbstractStrategy.h"
#include "LastTickMap.h"
using namespace Abstract;
class StrategyGrid :public AbstractStrategy
{
public:
	StrategyGrid();
	virtual ~StrategyGrid();
protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //�ɽ��ر�		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //������պ���
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
private:
	LastTickMap* lasttickmap;
	string lag1; //һ��
	string lag2; //����
	double centre; //����
	double gridgap; //������
	

};