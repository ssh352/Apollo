#include "stdafx.h"
#include "StrategyMA.h"
#include "RecordData.h"
#include "mmsystem.h"
#include <list>
#include <map>
#include "LOGGER.h"
#pragma comment(lib,"winmm.lib")

using namespace std;
using namespace CTPTOOL;

StrategyMA::StrategyMA(){
	this->subscribelist.push_back("ru1709");
	this->strategyname = "均线趋势策略";
	this->strategyid = 3006;
	this->BuyTryWrongTime = 0;
	this->SellTryWrongTime = 0;
	this->BuyOpen = 0;
	this->SellOpen = 0; 
	this->bFinish = 0;
	this->bDoneHalf = 0;
	this->BuyPos = 0;
	this->SellPos = 0;
	this->bSellWatch = 0;
	this->bBuyWatch = 0;
	this->DayCount1 = 0;
	this->DayCount2 = 0;
	this->bDoneHalf=0;
	this->SellCloseHalf=0;
	this->BuyCloseHalf=0;
	this->SellCloseAll=0;
	this->BuyCloseAll=0;
	this->bMiss = 0;
	this->bJudged = 0;
}

StrategyMA::~StrategyMA() {

}

void StrategyMA::OnRtnTrade(CThostFtdcTradeField *pTrade) {
	//利用交易回报，来观察持仓情况。
	if (pTrade->OffsetFlag == THOST_FTDC_OF_Open) {
		if (pTrade->Direction == THOST_FTDC_D_Buy) {
			BuyPos = BuyPos + pTrade->Volume;
			this->logger.get()->Log("当前时间%s，%s：开多单成功，此次成交%d手，当前多仓持有%d手。", pTrade->TradeDate, pTrade->TradeTime,pTrade->Volume, BuyPos);
			if (bDoneHalf == 1) {
				if (BuyPos < 15) {
					bFinish = 1;
				}
				else if (BuyPos == 15) {
					bFinish = 0;
					bDoneHalf = 0;
				}
			}
			else {
				if (BuyPos < 30) {
					bFinish = 1;
				}
				else if (BuyPos == 30) {
					bFinish = 0;
				}
			}
		}
		if (pTrade->Direction == THOST_FTDC_D_Sell) {
			SellPos = SellPos + pTrade->Volume;
			this->logger.get()->Log("当前时间%s，%s：开空单成功，此次成交%d手，当前空仓持有%d手。", pTrade->TradeDate, pTrade->TradeTime,pTrade->Volume, SellPos);
			if (bDoneHalf == 1) {
				if (SellPos < 15) {
					bFinish = -1;
				}
				else if (SellPos == 15) {
					bFinish = 0;
					bDoneHalf = 0;
				}
			}
			else {
				if (SellPos < 30) {
					bFinish = -1;
				}
				else if (SellPos == 30) {
					bFinish = 0;
				}
			}
		}
		//如果开仓没有全部成交，则继续报单直到开满。
		if (bFinish == 1&& bDoneHalf ==0) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeLockOrder(pTrade->InstrumentID, NowTick.BidPrice1, 30 - BuyPos, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			this->logger.get()->Log("当前时间%s，%s：补开多单报单，此次报单%d手，当前多仓持有%d手。", NowTick.TradingDay,NowTick.UpdateTime, 30 - BuyPos , BuyPos);
		}
		if (bFinish == -1&& bDoneHalf == 0) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeLockOrder(pTrade->InstrumentID, NowTick.AskPrice1, 30 - SellPos, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			this->logger.get()->Log("当前时间%s，%s：补开空单成功，此次报单%d手，当前空仓持有%d手。", NowTick.TradingDay, NowTick.UpdateTime, 30 - SellPos, SellPos);
		}
		if (bFinish == 1 && bDoneHalf == 1) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeLockOrder(pTrade->InstrumentID, NowTick.BidPrice1, 15 - BuyPos, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			this->logger.get()->Log("当前时间%s，%s：补开多单报单，此次报单%d手，当前多仓持有%d手。", NowTick.TradingDay, NowTick.UpdateTime, 15 - BuyPos, BuyPos);
		}
		if (bFinish == -1 && bDoneHalf == 1) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeLockOrder(pTrade->InstrumentID, NowTick.AskPrice1, 15 - SellPos, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			this->logger.get()->Log("当前时间%s，%s：补开空单成功，此次报单%d手，当前空仓持有%d手。", NowTick.TradingDay, NowTick.UpdateTime, 15 - SellPos, SellPos);
		}
	}

	if (pTrade->OffsetFlag == THOST_FTDC_OF_Close || pTrade->OffsetFlag == THOST_FTDC_OF_CloseToday || pTrade->OffsetFlag == THOST_FTDC_OF_CloseYesterday) {
		if (pTrade->Direction == THOST_FTDC_D_Buy) {
			SellPos = SellPos - pTrade->Volume;
			this->logger.get()->Log("当前时间%s，%s：平空单成功，此次成交%d手，当前空仓持有%d手。", pTrade->TradeDate, pTrade->TradeTime, pTrade->Volume, SellPos);
			if (bDoneHalf == 1) {
				if (SellPos > 15) {
					bFinish = 1;
				}
				if (SellPos == 15) {
					bFinish = 0;
					bDoneHalf == 0;
				}
			}
			else {
				if (SellPos > 0) {
					bFinish = 1;
				}
				if (SellPos == 0) {
					SellOpen = 0;
					bCross = 0;
					bFinish = 0;
					SellCloseAll = 0;
					SellCloseHalf = 0;
				}
			}
		}
		if (pTrade->Direction == THOST_FTDC_D_Sell) {
			BuyPos = BuyPos - pTrade->Volume;
			this->logger.get()->Log("当前时间%s，%s：平买单成功，此次成交%d手，当前多仓持有%d手。", pTrade->TradeDate, pTrade->TradeTime, pTrade->Volume, BuyPos);
			if (bDoneHalf == 1) {
				if (BuyPos > 15) {
					bFinish = 1;
				}
				if (BuyPos == 15) {
					bFinish = 0;
					bDoneHalf == 0;
				}
			}
			else {
				if (BuyPos > 0) {
					bFinish = 1;
				}
				if (BuyPos == 0) {
					BuyOpen = 0;
					bCross = 0;
					bFinish = 0;
					BuyCloseAll = 0;
					BuyCloseHalf = 0;
				}
			}
		}
		if (bFinish == 1) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeOrder(pTrade->InstrumentID, NowTick.BidPrice1, SellPos, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			this->logger.get()->Log("当前时间%s，%s：补平空单报单，此次报单%d手，当前空仓持有%d手。", NowTick.TradingDay, NowTick.UpdateTime, SellPos, SellPos);
		}
		if (bFinish == -1) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeOrder(pTrade->InstrumentID, NowTick.AskPrice1, BuyPos, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			this->logger.get()->Log("当前时间%s，%s：补平买单报单，此次报单%d手，当前多仓持有%d手。", NowTick.TradingDay, NowTick.UpdateTime, BuyPos, BuyPos);
		}
	}
}

void StrategyMA::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

void StrategyMA::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) {
	NowTick = *pDepthMarketData;
 	EMATemp10 = (2 * pDepthMarketData->LastPrice + (10 - 1)*EMA10) / (10 + 1);
	EMATemp20 = (2 * pDepthMarketData->LastPrice + (20 - 1)*EMA20) / (20 + 1);
	CString StrUpdateTime = pDepthMarketData->UpdateTime;
	CString StrTradingDay = pDepthMarketData->TradingDay;
	if (abs(EMATemp10 - EMATemp20) < 40) {
		bGlued = 1;
	}
	else {
		bGlued = 0;
	}
	
	//均线粘合的情况
	if (bGlued == 1) {
		//价格跌破20日线，且在20点以内，做空。
		if (SellOpen == 0 && SellTryWrongTime < 2 && pDepthMarketData->LastPrice < EMATemp20 && EMATemp20 - pDepthMarketData->LastPrice <= 20) {
			CThostFtdcInputOrderField inputOrderField;
			if (BuyPos == 0) {
				inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, 30, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			}
			else {
				inputOrderField = MakeLockOrder(pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, 30, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			}
			SellOpen = 1;
			this->logger.get()->Log("均线粘合，第%d次试错做空报单，当前价格为%f，EMA20为%f。", SellTryWrongTime+1, pDepthMarketData->LastPrice, EMATemp20);
		}
		//价格涨破20日线，且在20点以内，做多。
		if (BuyOpen == 0 && BuyTryWrongTime < 2 && pDepthMarketData->LastPrice > EMATemp20&&pDepthMarketData->LastPrice - EMATemp20 <= 20) {
			CThostFtdcInputOrderField inputOrderField;
			if (SellPos == 0) {
				inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			}
			else {
				inputOrderField = MakeLockOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			}
			BuyOpen = 1;
			this->logger.get()->Log("均线粘合，第%d次试错做多报单，当前价格为%f，EMA20为%f。", BuyTryWrongTime + 1, pDepthMarketData->LastPrice, EMATemp20);
		}
	}
	//均线不粘合的情况
	else {
		//20日线在10日线以下，价格跌破20日线且在20点以内，做空。
		if (EMATemp20 < EMATemp10) {
			if (SellOpen == 0 && SellTryWrongTime < 2 && pDepthMarketData->LastPrice < EMATemp20&&EMATemp20 - pDepthMarketData->LastPrice <= 20) {
				CThostFtdcInputOrderField inputOrderField;
				inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, 30, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				SellOpen = 1;
				this->logger.get()->Log("当前时间%s，%s：均线不粘合，第%d次试错做空报单，当前价格为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, SellTryWrongTime + 1, pDepthMarketData->LastPrice, EMATemp20);
			}
		}
		//20日线在10日线以上，价格涨破20日线且在20点以内，做多。
		if (EMATemp20 > EMATemp10) {
			if (BuyOpen == 0 && BuyTryWrongTime < 2 && pDepthMarketData->LastPrice > EMATemp20&&pDepthMarketData->LastPrice - EMATemp20 <= 20) {
				CThostFtdcInputOrderField inputOrderField;
				if (SellPos == 0) {
					inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
					int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				}
				else {
					inputOrderField = MakeLockOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
					int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				}
				BuyOpen = 1;
				this->logger.get()->Log("当前时间%s，%s：均线不粘合，第%d次试错做多报单，当前价格为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, BuyTryWrongTime + 1, pDepthMarketData->LastPrice, EMATemp20);
			}
		}
	}
	//卖单，当价格涨破20日线上20点，止损(买平)
	if (SellPos==30 && SellCloseAll==0 && pDepthMarketData->LastPrice >= EMATemp20 + 20) {
		CThostFtdcInputOrderField inputOrderField;
		inputOrderField = MakeCloseOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
		int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
		SellTryWrongTime++;
		SellCloseAll = 1;
		this->logger.get()->Log("当前时间%s，%s：第%d次买平止损报单，当前价格为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, SellTryWrongTime , pDepthMarketData->LastPrice, EMATemp20);
	}
	//买单，当价格跌破20日线下20点，止损(卖平)
	if (BuyPos == 30 && BuyCloseAll == 0 && pDepthMarketData->LastPrice <= EMATemp20 - 20) {
		CThostFtdcInputOrderField inputOrderField;
		inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
		int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
		BuyTryWrongTime++;
		BuyCloseAll = 1;
		this->logger.get()->Log("当前时间%s，%s：第%d次卖平止损报单，当前价格为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, BuyTryWrongTime , pDepthMarketData->LastPrice, EMATemp20);
	}
	//卖单，当价格涨上10日线50点时，止盈一半；当价格涨破20日线时，全平。
	if (SellPos == 30&& SellCloseHalf==0 && bGlued == 0) {
		if (pDepthMarketData->LastPrice < EMATemp10) {
			bCross = -1;
		}
		if (bCross == -1 && pDepthMarketData->LastPrice >= EMATemp10 + 50) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeCloseOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 15, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			bDoneHalf = 1;
			SellCloseHalf = 1;
			this->logger.get()->Log("当前时间%s，%s：第一次买平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice,EMATemp10, EMATemp20);
		}
	}
	if (SellPos == 15&&SellCloseAll==0 && bGlued == 0) {
		if (pDepthMarketData->LastPrice > EMATemp20) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeCloseOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 15, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			bDoneHalf = 1;
			SellCloseAll = 1;
			this->logger.get()->Log("当前时间%s，%s：第二次买平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
	}
	//买单，当价格跌下10日线20点，止盈一半；当价格跌破20日线，全平。
	if (BuyPos == 30&&BuyCloseHalf==0&&bGlued==0) {
		if (pDepthMarketData->LastPrice > EMATemp10) {
			bCross = 1;
		}
		if (bCross == 1 && pDepthMarketData->LastPrice <= EMATemp10 - 20) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeCloseOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 15, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			bDoneHalf = 1;
			BuyCloseHalf = 1;
			this->logger.get()->Log("当前时间%s，%s：第一次卖平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
	}
	if (BuyPos == 15&&BuyCloseAll==0 && bGlued == 0) {
		if (pDepthMarketData->LastPrice < EMATemp20) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeCloseOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 15, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			bDoneHalf = 1;
			BuyCloseAll = 1;
			this->logger.get()->Log("当前时间%s，%s：第二次卖平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
	}
	//判断收盘时的情况
	if (StrUpdateTime.Mid(0, 2).Compare("14") == 0 && StrUpdateTime.Mid(3, 2).Compare("55") == 0&& bJudged==0) {
		//如果有过跌破试错，但收盘站上20日线，则反向开多
		bJudged = 1;
		if (BuyOpen == 0 && SellTryWrongTime != 0 && pDepthMarketData->LastPrice > EMATemp20) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			BuyOpen = 1;
			this->logger.get()->Log("当前时间%s，%s：有过跌破试错，但收盘站上20日线，买开报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
		//如果有过涨破试错，但收盘价站下20日线，则反向开空
		if (SellOpen == 0 && BuyTryWrongTime != 0 && pDepthMarketData->LastPrice < EMATemp20) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, 30, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			SellOpen = 1;
			this->logger.get()->Log("当前时间%s，%s：有过涨破试错，但收盘站下20日线，卖开报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
		//第三次跌破20日线，且收盘价在20日线200点内，则观察。
		if (SellTryWrongTime == 2 && SellOpen==0&& bSellWatch==0 && pDepthMarketData->LastPrice < EMATemp20&&EMATemp20 - pDepthMarketData->LastPrice < 200) {
			bSellWatch = 1;
			bSellWatchDate = pDepthMarketData->TradingDay;
			this->logger.get()->Log("当前时间%s，%s：第三次跌破20日线，观察。当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
		//第三次涨破20日线，且收盘价在20日线200点内，则观察。
		if (BuyTryWrongTime == 2 && BuyOpen==0 && bBuyWatch == 0 && pDepthMarketData->LastPrice > EMATemp20&& pDepthMarketData->LastPrice - EMATemp20 < 200) {
			bBuyWatch = 1;
			bBuyWatchDate= pDepthMarketData->TradingDay;
			this->logger.get()->Log("当前时间%s，%s：第三次涨破20日线，观察。当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
		//有过试错，且收盘价在20日线上下200点以外，观察是否错过趋势。
		if (BuyTryWrongTime != 0 || SellTryWrongTime != 0) {
			if (SellOpen == 0 && bSellFar==0&& pDepthMarketData->LastPrice < EMATemp20 - 200) {
				bSellFar = 1;
				bSellFarDate = pDepthMarketData->TradingDay;
				Standard = (pDepthMarketData->ClosePrice + pDepthMarketData->OpenPrice) / 2.0;
				this->logger.get()->Log("当前时间%s，%s：向下远离的情况，当前价格为%f，EMA10为%f，EMA20为%f，比较标准为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20,Standard);
			}
			if (BuyOpen == 0 && bBuyFar==0&&pDepthMarketData->LastPrice > EMATemp20 + 200) {
				bBuyFar = 1;
				bBuyFarDate= pDepthMarketData->TradingDay;
				Standard = (pDepthMarketData->ClosePrice + pDepthMarketData->OpenPrice) / 2.0;
				this->logger.get()->Log("当前时间%s，%s：向上远离的情况，当前价格为%f，EMA10为%f，EMA20为%f，比较标准为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20, Standard);
			}
		}
	}
	//如果是在观察的情况下
	if (bSellWatch == 1) {
		//价格回到20日线50点内，开空。
		if (DayCount1 == 1 && SellOpen == 0 && pDepthMarketData->LastPrice < EMATemp20&&EMATemp20 - pDepthMarketData->LastPrice <= 50) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, 30, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			SellOpen = 1;
			this->logger.get()->Log("当前时间%s，%s：观察的情况，第二日回到20均线50点内，卖开报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
	}
	if (bBuyWatch == 1) {
		//价格回到20日线50点内，做多。
		if (DayCount1 == 1 && BuyOpen == 0 && pDepthMarketData->LastPrice > EMATemp20&&pDepthMarketData->LastPrice - EMATemp20 <= 50) {
			CThostFtdcInputOrderField inputOrderField;
			inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
			int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
			BuyOpen = 1;
			this->logger.get()->Log("当前时间%s，%s：观察的情况，第二日回到20均线50点内，买开报单，当前价格为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, EMATemp10, EMATemp20);
		}
	}
	//如果是在远离的情况下，判断是否错过趋势
	if (bSellFar == 1&&DayCount2==4) {
		if (pDepthMarketData->PreClosePrice < Standard) {
			bMiss = -1;
			this->logger.get()->Log("当前时间%s，%s：错过向下趋势，当前价格为%f，比较标准为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice,Standard, EMATemp10, EMATemp20);
		}
		bSellFar = 0;
	}
	if (bBuyFar == 1 && DayCount2 == 4) {
		if (pDepthMarketData->PreClosePrice > Standard) {
			bMiss = 1;
			this->logger.get()->Log("当前时间%s，%s：错过向上趋势，当前价格为%f，比较标准为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, Standard, EMATemp10, EMATemp20);
		}
		bBuyFar = 0;
	}
	//错过趋势之后，沿10日线，观察开单。
	if (bMiss == -1 && SellOpen == 0 && pDepthMarketData->LastPrice > EMATemp10 - 40) {
		CThostFtdcInputOrderField inputOrderField;
		inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->BidPrice1, 15, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
		int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
		bDoneHalf = 1;
		SellOpen = 1;
		this->logger.get()->Log("当前时间%s，%s：错过向下趋势，回到10日线50点内，卖开报单。当前价格为%f，比较标准为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime,  pDepthMarketData->LastPrice, Standard, EMATemp10, EMATemp20);
	}
	if (bMiss == 1 && BuyOpen == 0 && pDepthMarketData->LastPrice < EMATemp10 + 40) {
		CThostFtdcInputOrderField inputOrderField;
		inputOrderField = MakeOrder(pDepthMarketData->InstrumentID, pDepthMarketData->AskPrice1, 15, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
		int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
		bDoneHalf = 1;
		BuyOpen = 1;
		this->logger.get()->Log("当前时间%s，%s：错过向上趋势，回到10日线50点内，买开报单。当前价格为%f，比较标准为%f，EMA10为%f，EMA20为%f。", pDepthMarketData->TradingDay, pDepthMarketData->UpdateTime, pDepthMarketData->LastPrice, Standard, EMATemp10, EMATemp20);
	}
}

void StrategyMA::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {

}

void StrategyMA::OnRtnOrder(CThostFtdcOrderField *pOrder) {
	//如果撤单30手，或者15手，说明该单子一手都没成交。
	char i = pOrder->OrderSubmitStatus;
	if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled) {
		//平单的情况下，重新发单。
		if (SellPos == 30 && SellCloseAll == 1) {
			if (pOrder->VolumeTotalOriginal == 30) {
				CThostFtdcInputOrderField inputOrderField;
				inputOrderField = MakeCloseOrder(pOrder->InstrumentID, NowTick.AskPrice1, 30, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				this->logger.get()->Log("重新报单，第%d次买平止损报单，当前价格为%f，EMA20为%f。", SellTryWrongTime, NowTick.LastPrice, EMATemp20);
			}
		}
		if (SellPos == 30 && SellCloseAll == 0 && SellCloseHalf == 1) {
			if (pOrder->VolumeTotalOriginal == 15) {
				CThostFtdcInputOrderField inputOrderField;
				inputOrderField = MakeCloseOrder(pOrder->InstrumentID, NowTick.AskPrice1, 15, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				this->logger.get()->Log("重新报单，第一次买平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", NowTick.LastPrice, EMATemp10, EMATemp20);
			}
		}
		if (SellPos == 15 && SellCloseAll == 1) {
			if (pOrder->VolumeTotalOriginal == 15) {
				CThostFtdcInputOrderField inputOrderField;
				inputOrderField = MakeCloseOrder(pOrder->InstrumentID, NowTick.AskPrice1, 15, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				this->logger.get()->Log("重新报单，第二次买平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", NowTick.LastPrice, EMATemp10, EMATemp20);
			}
		}

		if (BuyPos == 30 && BuyCloseAll == 1) {
			if (pOrder->VolumeTotalOriginal == 30) {
				CThostFtdcInputOrderField inputOrderField;
				inputOrderField = MakeCloseOrder(pOrder->InstrumentID, NowTick.BidPrice1, 30, THOST_FTDC_D_Sell ,THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				this->logger.get()->Log("重新报单，当前价格为%f，EMA20为%f。", NowTick.LastPrice, EMATemp20);
			}
		}
		if (BuyPos == 30 && BuyCloseAll == 0 && BuyCloseHalf == 1) {
			if (pOrder->VolumeTotalOriginal == 15) {
				CThostFtdcInputOrderField inputOrderField;
				inputOrderField = MakeCloseOrder(pOrder->InstrumentID, NowTick.BidPrice1, 15, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				this->logger.get()->Log("重新报单，第一次卖平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", NowTick.LastPrice, EMATemp10, EMATemp20);
			}
		}
		if (BuyPos == 15 && BuyCloseAll == 1) {
			if (pOrder->VolumeTotalOriginal == 15) {
				CThostFtdcInputOrderField inputOrderField;
				inputOrderField = MakeCloseOrder(pOrder->InstrumentID, NowTick.BidPrice1, 15, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_IOC);
				int result = this->tdapi->ReqOrderInsert(&inputOrderField, this->requestid++);
				this->logger.get()->Log("重新报单，第二次卖平止盈报单，当前价格为%f，EMA10为%f，EMA20为%f。", NowTick.LastPrice, EMATemp10, EMATemp20);
			}
		}
		//开单的情况下，重新判断。
		if (SellPos == 0 && SellOpen == 1) {
			if (bMiss == -1) {
				if (pOrder->VolumeTotalOriginal == 15) {
					SellOpen = 0;
				}
			}
			if (bMiss == 0) {
				if (pOrder->VolumeTotalOriginal == 30) {
					SellOpen = 0;
				}
			}
		}
		if (BuyPos == 0 && BuyOpen == 1) {
			if (bMiss = 1) {
				if (pOrder->VolumeTotalOriginal == 15) {
					BuyOpen = 0;
				}
			}
			if (bMiss == 0) {
				if (pOrder->VolumeTotalOriginal == 30) {
					BuyOpen = 0;
				}
			}
		}
	}
}

void StrategyMA::OnInit() {
	EMA10 = this->parampersistencer->GetParameter("EMA10");
	EMA20 = this->parampersistencer->GetParameter("EMA20");
	//EMA10 = 10300.0;
	//EMA20 = 10215.0;
	this->logger.get()->Log("前一交易日的EMA10为%f，EMA20为%f", EMA10, EMA20);
}

void StrategyMA::OnTime(tm t){
	//每日开盘前，取出上一日的EMA。
	if (t.tm_hour == 20 && t.tm_min == 55) {
		//EMA10 = this->parampersistencer->GetParameter("EMA10");
		//EMA20 = this->parampersistencer->GetParameter("EMA20");
		/*this->logger.get()->Log("新交易日的EMA10为%f，EMA20为%f", EMA10, EMA20);*/
		if (DayCount1 == 1) {
			DayCount1 = 0;
			bSellWatch = 0;
			bBuyWatch = 0;
		}	
		if (DayCount2 == 4) {
			DayCount2 = 0;
		}
	}
	//判断是否是观察后第二天
	if (t.tm_hour == 21 && t.tm_min == 0&& bBuyWatch==1) {
		if (this->TradingDay != bBuyWatchDate) {
			DayCount1++;
			bBuyWatchDate = this->TradingDay;
		}
	}
	if (t.tm_hour == 21 && t.tm_min == 1 && bSellWatch == 1) {
		if (this->TradingDay != bSellWatchDate) {
			DayCount1++;
			bSellWatchDate = this->TradingDay;
		}
	}

	//判断远离情况下的交易日计数
	if (t.tm_hour == 21 && t.tm_min == 0 && bBuyFar == 1) {
		if (this->TradingDay != bBuyFarDate) {
			DayCount2++;
			bBuyFarDate = this->TradingDay;
		}
	}
	if (t.tm_hour == 21 && t.tm_min == 0 && bSellFar == 1) {
		if (this->TradingDay != bSellFarDate) {
			DayCount2++;
			bSellFarDate = this->TradingDay;
		}
	}

	//收盘后，将今日的EMA存入数据库
	if (t.tm_hour == 15 && t.tm_min == 30) {
		//EMA10 = EMATemp10;
		//EMA20 = EMATemp20;
		SellTryWrongTime = 0;
		BuyTryWrongTime = 0;
		bJudged = 0;
		this->parampersistencer->AddOrUpdateParameter("EMA10", this->EMATemp10);
		this->parampersistencer->AddOrUpdateParameter("EMA20", this->EMATemp20);
	}
}

CThostFtdcInputOrderField StrategyMA::MakeOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition) {
	CThostFtdcInputOrderField order;
	(void)memset(&order, 0, sizeof(order));
	strcpy(order.BrokerID, this->brokerid.c_str());
	strcpy(order.InvestorID, this->investid.c_str());
	strcpy(order.InstrumentID, instrumentid);
	sprintf(order.OrderRef, "%s", this->GetNewOrderRef().c_str());
	///强平原因: 非强平
	order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	order.IsAutoSuspend = 0;
	///用户强评标志: 否
	order.UserForceClose = 0;
	///投机套保标识：投机
	order.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
	///触发条件
	order.ContingentCondition = THOST_FTDC_CC_Immediately;
	///价格
	order.LimitPrice = price;
	///数量
	order.VolumeTotalOriginal = volume;
	///成交量类型
	order.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量
	order.MinVolume = 1;
	///买卖方向
	order.Direction = direction;
	///相反的方向	
	char opdir;
	if (direction == THOST_FTDC_D_Sell)
		opdir = THOST_FTDC_D_Buy;
	if (direction == THOST_FTDC_D_Buy)
		opdir = THOST_FTDC_D_Sell;
	///有效期类型
	order.TimeCondition = timecondition;
	///报单条件
	order.OrderPriceType = ordertype;
	///开仓类型
	order.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;
	///如果有仓位则为平仓单
	int allposition = this->portfoliomanager->GetPostionDetailVolume(instrumentid, opdir);
	int todayposition = this->portfoliomanager->GetPostionDetailVolumeToday(instrumentid, opdir, this->TradingDay);
	if (allposition != todayposition)
		int index = 0;
	if (allposition > 0) {
		/*if (opdir == THOST_FTDC_D_Buy)
		LOG("合约%s有多仓", instrumentid);
		if (opdir == THOST_FTDC_D_Sell)
		LOG("合约%s有空仓", instrumentid);
		LOG("TradingDay:%s", this->TradingDay);*/
		if (allposition - todayposition > 0)
			order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseYesterday;
		else
			order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseToday;
	}
	return order;
}

CThostFtdcInputOrderField StrategyMA::MakeLockOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition) {
	CThostFtdcInputOrderField order;
	(void)memset(&order, 0, sizeof(order));
	strcpy(order.BrokerID, this->brokerid.c_str());
	strcpy(order.InvestorID, this->investid.c_str());
	strcpy(order.InstrumentID, instrumentid);
	sprintf(order.OrderRef, "%s", this->GetNewOrderRef().c_str());
	///强平原因: 非强平
	order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	order.IsAutoSuspend = 0;
	///用户强评标志: 否
	order.UserForceClose = 0;
	///投机套保标识：投机
	order.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
	///触发条件
	order.ContingentCondition = THOST_FTDC_CC_Immediately;
	///价格
	order.LimitPrice = price;
	///数量
	order.VolumeTotalOriginal = volume;
	///成交量类型
	order.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量
	order.MinVolume = 1;
	///买卖方向
	order.Direction = direction;
	///有效期类型
	order.TimeCondition = timecondition;
	///报单条件
	order.OrderPriceType = ordertype;
	///开仓类型
	order.CombOffsetFlag[0] = THOST_FTDC_OFEN_Open;

	return order;
}

CThostFtdcInputOrderField StrategyMA::MakeCloseOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition) {
	CThostFtdcInputOrderField order;
	(void)memset(&order, 0, sizeof(order));
	strcpy(order.BrokerID, this->brokerid.c_str());
	strcpy(order.InvestorID, this->investid.c_str());
	strcpy(order.InstrumentID, instrumentid);
	sprintf(order.OrderRef, "%s", this->GetNewOrderRef().c_str());
	///强平原因: 非强平
	order.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///自动挂起标志: 否
	order.IsAutoSuspend = 0;
	///用户强评标志: 否
	order.UserForceClose = 0;
	///投机套保标识：投机
	order.CombHedgeFlag[0] = THOST_FTDC_HFEN_Speculation;
	///触发条件
	order.ContingentCondition = THOST_FTDC_CC_Immediately;
	///价格
	order.LimitPrice = price;
	///数量
	order.VolumeTotalOriginal = volume;
	///成交量类型
	order.VolumeCondition = THOST_FTDC_VC_AV;
	///最小成交量
	order.MinVolume = 1;
	///买卖方向
	order.Direction = direction;
	///有效期类型
	order.TimeCondition = timecondition;
	///报单条件
	order.OrderPriceType = ordertype;
	///开仓类型
	char opdir;
	if (direction == THOST_FTDC_D_Sell)
		opdir = THOST_FTDC_D_Buy;
	if (direction == THOST_FTDC_D_Buy)
		opdir = THOST_FTDC_D_Sell;
	//判断平今或平昨
	int allposition = this->portfoliomanager->GetPostionDetailVolume(instrumentid, opdir);
	int todayposition = this->portfoliomanager->GetPostionDetailVolumeToday(instrumentid, opdir, this->TradingDay);
	if (allposition > 0) {
		if (allposition - todayposition > 0)
			order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseYesterday;
		else
			order.CombOffsetFlag[0] = THOST_FTDC_OFEN_CloseToday;
	}

	return order;
}

time_t StrategyMA::GetTimeStamp(string date, string time) {
	tm tm;
	int year, month, day, hour, minute, second;
	year = atoi((date.substr(0, 4)).c_str());
	month = atoi((date.substr(4, 2)).c_str());
	day = atoi((date.substr(6, 2)).c_str());
	hour = atoi((time.substr(0, 2)).c_str());
	minute = atoi((time.substr(3, 2)).c_str());
	second = atoi((time.substr(6, 2)).c_str());
	tm.tm_year = year - 1900;
	tm.tm_mon = month - 1;
	tm.tm_mday = day;
	tm.tm_hour = hour;
	tm.tm_min = minute;
	tm.tm_sec = second;
	tm.tm_isdst = 0;
	time_t t = mktime(&tm); ///已经减了8个时区  
	return t; ///秒时间 	
}