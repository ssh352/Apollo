#pragma once
#include "AbstractStrategy.h"
#include "KBarGenerater.h"

using namespace Abstract;
using namespace std;
/***ͻ�Ʊ�������
A.����10k��20k��30k�ĸߵͼۡ�
B.�ж��Ƿ���ƽ̨��
C.����ƽ̨����ͻ��ʱ������
***/
class StrategyBreakAlert : public AbstractStrategy
{
public:
	StrategyBreakAlert();
	~StrategyBreakAlert();

private:
	KBarGenerater * kbargen;
	int flag;
	double LasttickVolume;			//��һ�����
	CString AlertTime;

protected:
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //�ɽ��ر�		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //������պ���
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
};