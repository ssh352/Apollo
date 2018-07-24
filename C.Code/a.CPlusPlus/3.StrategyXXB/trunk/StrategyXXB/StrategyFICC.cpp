#include "stdafx.h"
#include "StrategyFICC.h"
#include <iostream>  
#include <math.h>
#include <string> 
#include <time.h>
#include <vector>  
#include <fstream>  
#include <sstream>  

using namespace std;
using namespace Abstract;

StrategyFICC::StrategyFICC()
{
	this->strategyid = 6002;
	this->strategyname = "展期执行策略";
	this->version = "0.1";
	this->desc = "用于执行展期策略下的单";
	this->author = "夏献霸";
	this->needtradelong = false;
	this->needtradeshort = false;
	this->hascompletetradeshort = false;
	this->hascompletetradelong = false;
	this->singglemount = 300000;//单个品种的保证金数量
	this->lasttradingtime = 0;
	this->lastpythonruntime = 0;
	memset(lastradelongday, 0, sizeof(lastradelongday));
	memset(lastradeshortday, 0, sizeof(lastradeshortday));

}


StrategyFICC::~StrategyFICC()
{
}

void StrategyFICC::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
}
void StrategyFICC::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void StrategyFICC::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	CThostFtdcQryOrderField *pOrder = NULL;
	int nRequestID = 6002;
	bool bIsLast = true;;
	if (this->timer->GetTmCurrentTime().tm_yday == this->timer->GetTmTime(lasttradingtime).tm_yday) {   //如果是同一天生成的那么下单
		this->TradeLong(pDepthMarketData);
		this->TradeShort(pDepthMarketData);
	}
	//int result = this->tdapi->ReqQryOrder(pOrder, nRequestID);
}
void  StrategyFICC::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void  StrategyFICC::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

}
void StrategyFICC::OnInit()
{
	shared_ptr<byte> lasttradingtime1 = this->parampersistencer->GetObjectParameter("lasttradingtime", sizeof(time_t));
	memcpy(&lasttradingtime, lasttradingtime1.get(), sizeof(time_t));
 
}

void StrategyFICC::OnRspQryOrder(CThostFtdcOrderField *pOrder, int nRequestID, bool bIsLast)
{

}

void StrategyFICC::OnRspQryTrade(CThostFtdcTradeField *pTrade, int nRequestID, bool bIsLast)
{
	
}

void StrategyFICC::OnTime(tm t)
{
// 从下午一点30 每隔5分钟去读取一次 开始读从文件里读数据如果读到数据那么
	//if (needtradelong==false&&(t.tm_hour >= 9&&t.tm_hour<15)&&t.tm_min==5&&t.tm_sec==0) { //5


	
	if (((this->lasttradingtime > 0 && mktime(&t) > this->lasttradingtime + 60 * 60 * 24 * 5)) || this->lasttradingtime == 0) {
		this->PyRun();

	}
		if (needtradelong == false && (t.tm_hour >= 9 && t.tm_hour<15)) { //5
			
		needtradelong = this->IsNeedTradeLong();
		
		memcpy(lastradelongday, this->TradingDay, sizeof(lastradelongday));
	}
	if (needtradelong==true) {

		if(strcmp(lastradelongday, this->lastradelongday)!=0)//如果tradingday 已经不相同了那么需要重新来读取数据
		{
			this->needtradelong = false;
			this->hascompletetradelong = false;
		}
	}

	//if (needtradeshort == false && (t.tm_hour >= 9 && t.tm_hour<15) && t.tm_min == 5 && t.tm_sec == 0) { //5
		if (needtradeshort == false && (t.tm_hour >= 9 && t.tm_hour<15)) { //5
		needtradeshort = this->IsNeedTradeShort();

		memcpy(lastradeshortday, this->TradingDay, sizeof(lastradeshortday));
	}

	if (needtradeshort == true) {

		if (strcmp(lastradeshortday, this->lastradeshortday) != 0)//如果tradingday 已经不相同了那么需要重新来读取数据
		{
			this->needtradeshort = false;
			this->hascompletetradeshort = false;
		}

	}
	if (needtradelong || needtradeshort) {

		this->lasttradingtime = mktime(&t);
		shared_ptr<byte> b = shared_ptr<byte>(new byte[sizeof(time_t)], [](byte *p) {delete[] p; });
	
		memcpy(b.get(), &lasttradingtime, sizeof(time_t));
		this->parampersistencer->AddOrUpdataParameter("lasttradingtime", b, sizeof(time_t));

	}
	//if(t.tm_hour>=15&&t.tm_min==5)

}


void StrategyFICC::TradeLong(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if ((needtradelong&&hascompletetradelong) || needtradelong == false)
		return;


	bool tempneedtrade = false;

	for (string instrumentid : list_longpositon) { //去掉点后面的
		string ctpinstrumentid = instrumentid.substr(0, instrumentid.find('.'));
		if (strcmp(ctpinstrumentid.c_str(), pDepthMarketData->InstrumentID) != 0)
		{
			tempneedtrade = true;
			continue;
		}
		CThostFtdcInstrumentField  instrument=	this->instrumentmap->getInstrumentid(ctpinstrumentid);
		double multi = instrument.VolumeMultiple; //合约乘数
		double marginratio = instrument.LongMarginRatio;  //多头保证金率

		//int volume = (this->singglemount / (pDepthMarketData->LastPrice*multi*marginratio));
		int volume = (this->singglemount / (pDepthMarketData->LastPrice*multi));
		if (this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Buy)<volume && this->portfoliomanager->GetOrderInputCount(ctpinstrumentid) == 0 && this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Buy) == 0)
		{

			//如果时间在2:50之前 那么

		//	this->instrumentmap[]

			CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)ctpinstrumentid.c_str(), pDepthMarketData->UpperLimitPrice, volume - this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Buy), THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);
			int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
			tempneedtrade = true;

		}
		int needclosevolume = this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Buy) - volume;
		if (this->portfoliomanager->GetPostionDetailVolumeYesterday(ctpinstrumentid, THOST_FTDC_D_Buy, pDepthMarketData->TradingDay) > needclosevolume&&this->portfoliomanager->GetOrderInputCount(ctpinstrumentid) == 0 && this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Buy) == 0) {

			CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)ctpinstrumentid.c_str(), pDepthMarketData->LowerLimitPrice, needclosevolume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Close);
			int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
			tempneedtrade = true;
		}

	}
	// 
	vector<CThostFtdcInvestorPositionDetailField> list_postiondetail = this->portfoliomanager->GetPostionDetail(THOST_FTDC_D_Buy);

	for (auto detail : list_postiondetail) {

		if (strcmp(detail.InstrumentID, pDepthMarketData->InstrumentID) == 0)
		{

		
		if (!this->IsInLongList(detail.InstrumentID)) {
			//平掉该单子	

			int volume = this->portfoliomanager->GetPostionDetailVolume(detail.InstrumentID, THOST_FTDC_D_Buy);
			CThostFtdcInputOrderField orderinput = this->CreateOrder(detail.InstrumentID, pDepthMarketData->LowerLimitPrice,  this->portfoliomanager->GetPostionDetailVolume(detail.InstrumentID, THOST_FTDC_D_Buy), THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_CloseYesterday);
			int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);

		
			tempneedtrade = true;



		}
		break;
		}

	}


	



}
void StrategyFICC::TradeShort(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if ((needtradeshort&&hascompletetradeshort) || needtradeshort == false)
		return;

	bool tempneedtrade = false;

	for (string instrumentid : list_shortpositon) { //去掉点后面的
		string ctpinstrumentid = instrumentid.substr(0, instrumentid.find('.'));
		if (strcmp(ctpinstrumentid.c_str(), pDepthMarketData->InstrumentID) != 0)
			continue;
		CThostFtdcInstrumentField  instrument = this->instrumentmap->getInstrumentid(ctpinstrumentid);
		double multi = instrument.VolumeMultiple; //合约乘数
		double marginratio = instrument.ShortMarginRatio;  //多头保证金率

		//int volume = (this->singglemount / (pDepthMarketData->LastPrice*multi*marginratio));
		int volume = (this->singglemount / (pDepthMarketData->LastPrice*multi));
		this->logger->Log("%s volume %d \n", ctpinstrumentid.c_str(), volume);
		this->logger->Log("%s postion %d \n", ctpinstrumentid.c_str(), this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell));
		this->logger->Log("%s orderinputcount %d \n", ctpinstrumentid.c_str(), this->portfoliomanager->GetOrderInputCount(ctpinstrumentid));
		this->logger->Log("%s ordercount %d \n", ctpinstrumentid.c_str(), this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Sell));
		if (this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell)<volume && this->portfoliomanager->GetOrderInputCount(ctpinstrumentid) == 0 && this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Sell) == 0)
		{

			//如果时间在2:50之前 那么

			CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)ctpinstrumentid.c_str(), pDepthMarketData->LowerLimitPrice, volume - this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell), THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);
			int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
			tempneedtrade = true;

		}
		if (this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell)>volume && this->portfoliomanager->GetOrderInputCount(ctpinstrumentid) == 0 && this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Buy) == 0)
		{

			//如果时间在2:50之前 那么
			int needclosevolume = this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell) - volume;
			if (this->portfoliomanager->GetPostionDetailVolumeYesterday(ctpinstrumentid, THOST_FTDC_D_Sell, pDepthMarketData->TradingDay) > needclosevolume) {

				CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)ctpinstrumentid.c_str(), pDepthMarketData->UpperLimitPrice, needclosevolume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Close);
				int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
				tempneedtrade = true;
			}
		

		}

	}
	// 
	vector<CThostFtdcInvestorPositionDetailField> list_postiondetail = this->portfoliomanager->GetPostionDetail(THOST_FTDC_D_Sell);

	for (auto detail : list_postiondetail) {
		if (strcmp(detail.InstrumentID, pDepthMarketData->InstrumentID) == 0)
		{
			if (!this->IsInShortList(detail.InstrumentID)) {
				//平掉该单子			
				int volume = this->portfoliomanager->GetPostionDetailVolume(detail.InstrumentID, THOST_FTDC_D_Sell);
				CThostFtdcInputOrderField orderinput = this->CreateOrder(detail.InstrumentID, pDepthMarketData->UpperLimitPrice, this->portfoliomanager->GetPostionDetailVolume(detail.InstrumentID, THOST_FTDC_D_Sell), THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_CloseYesterday);

				int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
				tempneedtrade = true;



			}
			break;

		}


	}


	if (this->portfoliomanager->GetOrderCount() == 0 && this->portfoliomanager->GetOrderInputCount() == 0 && tempneedtrade == false)
	{

		//hascompletetradeshort = true;
	}




}


/****************************************************************
Date: 2017-11-06
Author: xia xianba
Copyright: HeXin Invest Inc.
Description: 
  - Input: The CSV file containing transaction information
  - Output: A vector contains csv file after parsing
****************************************************************/
vector<string> StrategyFICC::GetPostionListFromFile(string files)
{
	string str;
	string lineStr;
	stringstream ss;

	vector<string> lineArray;
	ifstream inFile(files, ios::in);

	while (getline(inFile, lineStr))
	{
		ss.str(lineStr);
	}

	while (getline(ss, str, ','))
		lineArray.push_back(str);
	inFile.close();
	for (int i = 0; i < lineArray.size(); i++)
		this->logger->Log("LineArray[%d]: %s. \n", i, lineArray[i].c_str());

	return lineArray;
}

/****************************************************************
Date: 2017-11-06
Author: xia xianba
Copyright: HeXin Invest Inc.
Description:
- Input: Current date
- Output: If trading today,ture or false
****************************************************************/
bool StrategyFICC::IsNeedTradeLong()
{
	time_t tm_list;
	time_t tm_now;
	string lastdate;
	string longfile = "long.csv";	
	
	list_longpositon = this->GetPostionListFromFile(longfile);
	lastdate = list_longpositon[0];
	tm_list = StringToDatetime(list_longpositon[0]);
	tm_now = this->timer->GetTCurrentTime();
	list_longpositon.erase(list_longpositon.begin()); //删除日期。
	double ms = difftime(tm_now, tm_list);
	double days = ceil(ms / 86400);
	//if (days <= 2)
		return true;
	//else
	//	return false;
}



bool StrategyFICC::IsNeedTradeShort()
{

	time_t tm_list;
	time_t tm_now;
	string lastdate;
	string shortfile = "short.csv";
	list_shortpositon = this->GetPostionListFromFile(shortfile);
	lastdate = list_shortpositon[0];
	tm_list = StringToDatetime(list_shortpositon[0]);
	tm_now = this->timer->GetTCurrentTime();
	list_shortpositon.erase(list_shortpositon.begin()); //删除日期。
	double ms = difftime(tm_now, tm_list);
	double days = ceil(ms / 86400);
	//if (days <= 2)
		return true;
	//else
		//return false;



}
//是不是在多开列表
bool StrategyFICC::IsInLongList(string instrumentid)
{
	/*vecotr<> list_postion=*/
	bool rvalue = false;	
	for (string pinstrumentid :list_longpositon) {
		if (strstr(pinstrumentid.c_str(),instrumentid.c_str())!=0) {//因为带有
			rvalue = true;
			break;

		}

	}
	return rvalue;

}
bool StrategyFICC::IsInShortList(string instrumentid)

{
	bool rvalue = false;	
	for (string pinstrumentid : list_shortpositon) {

		if (strstr(pinstrumentid.c_str(), instrumentid.c_str())!=0) {

			rvalue = true;
			break;

		}

	}

	return rvalue;
}


/****************************************************************
Date: 2017-11-06
Author: xia xianba
Copyright: HeXin Invest Inc.
Description:
- Input: The string format of the date
- Output: timestamp in linux format
****************************************************************/
time_t StrategyFICC::StringToDatetime(string str)
{
	tm tm_;
	int year, month, day, hour = 0, minute = 0, second = 0;
	sscanf(str.c_str(), "%d-%d-%d ", &year, &month, &day);
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;
	time_t t_ = mktime(&tm_);
	return t_;
}

/****************************************************************
Date: 2017-11-21
Author: xia xianba
Copyright: HeXin Invest Inc.
Description: Run Python Program
****************************************************************/
void StrategyFICC::PyRun()
{
	fstream file;
	file.open("python/SimTrade.py", ios::in);
	if(file)
	{
		system("python python/SimTrade.py");
	}
}
