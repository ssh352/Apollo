#include "stdafx.h"
#include "StrategyMA40Alert.h"
#include "RecordData.h"
#include "mmsystem.h"
#include <list>
#include <map>
#include "LOGGER.h"
#pragma comment(lib,"winmm.lib")

//#include "InStrumentMap.h"
using namespace std;
using namespace CTPTOOL;

StrategyMA40Alert::StrategyMA40Alert() {
	this->strategyid = 3007;
	this->strategyname = "40日均线提醒";
	this->subscribelist.push_back("l1709");
	this->subscribelist.push_back("pp1709");
	this->flag = 0;
}

StrategyMA40Alert::~StrategyMA40Alert() {

}

void StrategyMA40Alert::OnTime(tm t) {
	//收盘后，将今日的EMA存入数据库
	if (t.tm_hour == 15 && t.tm_min == 30) {
		this->parampersistencer->AddOrUpdateParameter("LEMA10", this->m_RecordData["l1709"].EMATemp10);
		this->parampersistencer->AddOrUpdateParameter("LEMA40", this->m_RecordData["l1709"].EMATemp40);
		this->parampersistencer->AddOrUpdateParameter("PPEMA10", this->m_RecordData["pp1709"].EMATemp10);
		this->parampersistencer->AddOrUpdateParameter("PPEMA40", this->m_RecordData["pp1709"].EMATemp40);
	}
}

void StrategyMA40Alert::OnRtnTrade(CThostFtdcTradeField *pTrade) {

}

void StrategyMA40Alert::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {


}

void StrategyMA40Alert::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	CString Id = pDepthMarketData->InstrumentID;
	if (Id.Compare("l1709")==0) {
		m_RecordData[pDepthMarketData->InstrumentID].EMATemp10= (2 * pDepthMarketData->LastPrice + (10 - 1)*m_RecordData[pDepthMarketData->InstrumentID].EMA10) / (10 + 1);
		m_RecordData[pDepthMarketData->InstrumentID].EMATemp40= (2 * pDepthMarketData->LastPrice + (40 - 1)*m_RecordData[pDepthMarketData->InstrumentID].EMA40) / (40 + 1);
		//10日线小于40日线，且差值大于50，跌破后回踩。提醒。
		if (m_RecordData[pDepthMarketData->InstrumentID].EMATemp10 < m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&&m_RecordData[pDepthMarketData->InstrumentID].EMATemp40 - m_RecordData[pDepthMarketData->InstrumentID].EMATemp10>50) {
			if (pDepthMarketData->LastPrice >= m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&& flag!=-1) {
				this->logger.get()->Log("%s向上回踩至40日线,40日均线为%f,当前价格 %f。", pDepthMarketData->InstrumentID, m_RecordData[pDepthMarketData->InstrumentID].EMATemp40, pDepthMarketData->LastPrice);
				sndPlaySound("提示下单.wav", SND_ASYNC);
				flag = -1;
				m_RecordData[pDepthMarketData->InstrumentID].AlertTime = pDepthMarketData->UpdateTime;
			}
		}
		//10日线大于40日线，且差值大于50，涨破后回踩，提醒。
		if (m_RecordData[pDepthMarketData->InstrumentID].EMATemp10 > m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&&m_RecordData[pDepthMarketData->InstrumentID].EMATemp10 - m_RecordData[pDepthMarketData->InstrumentID].EMATemp40 > 50) {
			if (pDepthMarketData->LastPrice <= m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&&flag!=1) {
				this->logger.get()->Log("%s向下回踩至40日线,40日均线为%f,当前价格 %f。", pDepthMarketData->InstrumentID, m_RecordData[pDepthMarketData->InstrumentID].EMATemp40, pDepthMarketData->LastPrice);
				sndPlaySound("提示下单.wav", SND_ASYNC);
				flag = 1;
				m_RecordData[pDepthMarketData->InstrumentID].AlertTime = pDepthMarketData->UpdateTime;
			}
		}
	}
	if (Id.Compare("pp1709") == 0) {
		m_RecordData[pDepthMarketData->InstrumentID].EMATemp10 = (2 * pDepthMarketData->LastPrice + (10 - 1)*m_RecordData[pDepthMarketData->InstrumentID].EMA10) / (10 + 1);
		m_RecordData[pDepthMarketData->InstrumentID].EMATemp40 = (2 * pDepthMarketData->LastPrice + (40 - 1)*m_RecordData[pDepthMarketData->InstrumentID].EMA40) / (40 + 1);
		//10日线小于40日线，且差值大于50，跌破后回踩。提醒。
		if (m_RecordData[pDepthMarketData->InstrumentID].EMATemp10 < m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&&m_RecordData[pDepthMarketData->InstrumentID].EMATemp40 - m_RecordData[pDepthMarketData->InstrumentID].EMATemp10>50) {
			if (pDepthMarketData->LastPrice >= m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&&flag!=-2) {
				this->logger.get()->Log("%s向上回踩至40日线,40日均线为%f,当前价格 %f。", pDepthMarketData->InstrumentID, m_RecordData[pDepthMarketData->InstrumentID].EMATemp40, pDepthMarketData->LastPrice);
				sndPlaySound("提示下单.wav", SND_ASYNC);
				flag = -2;
				m_RecordData[pDepthMarketData->InstrumentID].AlertTime = pDepthMarketData->UpdateTime;
			}
		}
		//10日线大于40日线，且差值大于50，涨破后回踩，提醒。
		if (m_RecordData[pDepthMarketData->InstrumentID].EMATemp10 > m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&&m_RecordData[pDepthMarketData->InstrumentID].EMATemp10 - m_RecordData[pDepthMarketData->InstrumentID].EMATemp40 > 50) {
			if (pDepthMarketData->LastPrice <= m_RecordData[pDepthMarketData->InstrumentID].EMATemp40&&flag!=2) {
				this->logger.get()->Log("%s向下回踩至40日线,40日均线为%f,当前价格 %f。", pDepthMarketData->InstrumentID, m_RecordData[pDepthMarketData->InstrumentID].EMATemp40, pDepthMarketData->LastPrice);
				sndPlaySound("提示下单.wav", SND_ASYNC);
				flag = 2;
				m_RecordData[pDepthMarketData->InstrumentID].AlertTime = pDepthMarketData->UpdateTime;
			}
		}
	}

	if ((flag == 1 || flag == -1) && Id.Compare("l1709") == 0) {
		CString StrUpdateTime = pDepthMarketData->UpdateTime;
		int time1, time2, time3, time4;
		time1 = atoi(m_RecordData[pDepthMarketData->InstrumentID].AlertTime.Mid(3, 1));
		time2 = atoi(StrUpdateTime.Mid(3, 1));
		time3 = atoi(m_RecordData[pDepthMarketData->InstrumentID].AlertTime.Mid(4, 1));
		time4 = atoi(StrUpdateTime.Mid(4, 1));
		if (time1 == time2) {
			if (time4 - time3 == 5) {
				flag = 0;
			}
		}
		else
		{
			if (time3 - time4 == 5) {
				flag = 0;
			}
		}

	}

	if ((flag == 2 || flag == -2) && Id.Compare("pp1709") == 0) {
		CString StrUpdateTime = pDepthMarketData->UpdateTime;
		int time1, time2, time3, time4;
		time1 = atoi(m_RecordData[pDepthMarketData->InstrumentID].AlertTime.Mid(3, 1));
		time2 = atoi(StrUpdateTime.Mid(3, 1));
		time3 = atoi(m_RecordData[pDepthMarketData->InstrumentID].AlertTime.Mid(4, 1));
		time4 = atoi(StrUpdateTime.Mid(4, 1));
		if (time1 == time2) {
			if (time4 - time3 == 5) {
				flag = 0;
			}
		}
		else
		{
			if (time3 - time4 == 5) {
				flag = 0;
			}
		}

	}


}

void StrategyMA40Alert::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {


}

void StrategyMA40Alert::OnRtnOrder(CThostFtdcOrderField *pOrder) {


}

void StrategyMA40Alert::OnInit() {
	m_RecordData["l1709"].EMA10 = this->parampersistencer->GetParameter("LEMA10");
	m_RecordData["l1709"].EMA40 = this->parampersistencer->GetParameter("LEMA40");
	m_RecordData["pp1709"].EMA10 = this->parampersistencer->GetParameter("PPEMA10");
	m_RecordData["pp1709"].EMA40 = this->parampersistencer->GetParameter("PPEMA40");

	this->logger.get()->Log("L1709前一交易日的EMA10为%f，EMA40为%f", m_RecordData["l1709"].EMA10, m_RecordData["l1709"].EMA40);
	this->logger.get()->Log("PP1709前一交易日的EMA10为%f，EMA40为%f", m_RecordData["pp1709"].EMA10, m_RecordData["pp1709"].EMA40);
}


