#include "stdafx.h"
#include "StrategyBreakAlert.h"
#include "mmsystem.h"
#include <list>
#include "LOGGER.h"


#pragma comment(lib,"winmm.lib")

using namespace std;
using namespace CTPTOOL;

StrategyBreakAlert::StrategyBreakAlert() {
	this->strategyid = 2000;
	this->strategyname = "突破平台预警";
	this->subscribelist.push_back("ru1801");
	kbargen = new KBarGenerater();
	flag = 0;
	LasttickVolume = 0.0;
}

StrategyBreakAlert::~StrategyBreakAlert() {

	delete kbargen;
}

void StrategyBreakAlert::OnTime(tm t) {
	
}

void StrategyBreakAlert::OnRtnTrade(CThostFtdcTradeField *pTrade) {

}

void StrategyBreakAlert::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {


}

void StrategyBreakAlert::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	
	kbargen->OnRtnDepthMarketData(pDepthMarketData);
	double High10 = 0.0, High20 = 0.0, High50 = 0.0, High100 = 0.0, Low10 = 999999.0, Low20 = 99999.0, Low50 = 999999.0, Low100 = 999999.0;
	//list<TKDATA>::iterator iter;
	auto list = kbargen->GetBarList(1);
	int bar_count = list.size();

	if (bar_count >=10) {
 		int i = 0;
		double temp = 0.0;
		for (i = bar_count-1; i >= bar_count-10; i--) {
			TKDATA k = list[i];
			temp = k.high;
			if (temp > High10) {
				High10 = temp;
			}
			temp = k.low;
			if (temp < Low10) {
				Low10 = temp;
			} 
		}
	}

	if (bar_count >= 20) {
		int i = 0;
		double temp = 0.0;
		for (i = bar_count - 1; i >= bar_count - 20; i--) {
			TKDATA k = list[i];
			temp = k.high;
			if (temp > High20) {
				High20 = temp;
			}
			temp = k.low;
			if (temp < Low20) {
				Low20 = temp;
			}
		}
	}

	if (bar_count >= 50) {
		int i = 0;
		double temp = 0.0;
		for (i = bar_count - 1; i >= bar_count - 50; i--) {
			TKDATA k = list[i];
			temp = k.high;
			if (temp > High50) {
				High50 = temp;
			}
			temp = k.low;
			if (temp < Low50) {
				Low50 = temp;
			}
		}
	}

	if (bar_count >= 100) {
		int i = 0;
		double temp = 0.0;
		for (i = bar_count - 1; i >= bar_count - 100; i--) {
			TKDATA k = list[i];
			temp = k.high;
			if (temp > High100) {
				High100 = temp;
			}
			temp = k.low;
			if (temp < Low100) {
				Low100 = temp;
			}
		}
	}

	//触发警告逻辑
	if ((High10 - Low10)<=30 && bar_count >= 10) {
		if (pDepthMarketData->LastPrice > High10 || pDepthMarketData->LastPrice < Low10 && flag==0 ) {
 			this->logger.get()->Log( "%s突破10周期平台,平台低价 %f,平台高价 %f,当前价格 %f,当前tick的量 %f。", pDepthMarketData->InstrumentID, Low10, High10, pDepthMarketData->LastPrice, (pDepthMarketData->Volume - LasttickVolume));
			sndPlaySound("提示下单.wav", SND_ASYNC);
			flag = 1;
			CString AlertTime = pDepthMarketData->UpdateTime;
		}
	}


	if ((High20 - Low20)<=50&& bar_count >= 20) {
		if (pDepthMarketData->LastPrice > High20 || pDepthMarketData->LastPrice < Low20 && flag!=2 && flag!=3 && flag!=4) {
			this->logger.get()->Log("%s突破20周期平台,平台低价 %f,平台高价 %f,当前价格 %f,当前tick的量 %f。", pDepthMarketData->InstrumentID, Low20, High20, pDepthMarketData->LastPrice, (pDepthMarketData->Volume-LasttickVolume));
			sndPlaySound("提示下单.wav", SND_ASYNC); 
			flag = 2;
			CString AlertTime = pDepthMarketData->UpdateTime;
		}
	}

	if ((High50 - Low50)<=100&& bar_count >= 50) {
		if (pDepthMarketData->LastPrice > High50 || pDepthMarketData->LastPrice < Low50 && flag!=3 && flag!=4) {
			this->logger.get()->Log("%s突破50周期平台,平台低价 %f,平台高价 %f,当前价格 %f,当前tick的量 %f。", pDepthMarketData->InstrumentID, Low50, High50, pDepthMarketData->LastPrice, (pDepthMarketData->Volume - LasttickVolume));
			sndPlaySound("提示下单.wav", SND_ASYNC);
			flag = 3;
			CString AlertTime = pDepthMarketData->UpdateTime;
		}
	}

	if ((High100 - Low100)<=(pDepthMarketData->PreSettlementPrice*0.01) && bar_count >= 100) {
		if (pDepthMarketData->LastPrice > High100 || pDepthMarketData->LastPrice < Low100&&flag != 4) {
			this->logger.get()->Log("%s突破100周期平台,平台低价 %f,平台高价 %f,当前价格 %f,当前tick的量 %f。", pDepthMarketData->InstrumentID, Low100, High100, pDepthMarketData->LastPrice, (pDepthMarketData->Volume - LasttickVolume));
			sndPlaySound("提示下单.wav", SND_ASYNC);
			flag = 4;
			CString AlertTime = pDepthMarketData->UpdateTime;
		}
	}


	if (flag != 0) {
		CString StrUpdateTime = pDepthMarketData->UpdateTime;
		int time1, time2, time3, time4;
		time1 = atoi(AlertTime.Mid(3, 1));
		time2 = atoi(StrUpdateTime.Mid(3, 1));
		time3 = atoi(AlertTime.Mid(4, 1));
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

	LasttickVolume = pDepthMarketData->Volume;
}

void StrategyBreakAlert::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {


}

void StrategyBreakAlert::OnRtnOrder(CThostFtdcOrderField *pOrder) {


}

void StrategyBreakAlert::OnInit() {


}