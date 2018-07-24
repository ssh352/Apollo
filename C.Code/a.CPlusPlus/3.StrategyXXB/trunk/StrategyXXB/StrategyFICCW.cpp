#include "stdafx.h"
#include "StrategyFICCW.h"
#include <iostream>  
#include <math.h>
#include <string> 
#include <time.h>
#include <vector>  
#include <fstream>  
#include <sstream>  

using namespace std;
using namespace Abstract;

StrategyFICCW::StrategyFICCW()
{
	this->strategyid = 6008;
	this->strategyname = "ÿ�ջ���չ�ڲ���";
	this->version = "1.0";
	this->desc = "չ�ڶ����ڵ��ֲ���";
	this->author = "HeXin";
	this->needtradelong = false;
	this->needtradeshort = false;
	this->hascompletetradeshort = false;
	this->hascompletetradelong = false;
	this->singglemount = 30000;//����Ʒ�ֵı�֤������
	this->lasttradingtime = 0;
	memset(lastradelongday, 0, sizeof(lastradelongday));
	memset(lastradeshortday, 0, sizeof(lastradeshortday));

}


StrategyFICCW::~StrategyFICCW()
{
}

void StrategyFICCW::OnRtnTrade(CThostFtdcTradeField *pTrade)
{
}
void StrategyFICCW::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void StrategyFICCW::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	CThostFtdcQryOrderField *pOrder = NULL;
	if (this->timer->GetTmCurrentTime().tm_yday == this->timer->GetTmTime(lasttradingtime).tm_yday) {   //�����ͬһ�����ɵ���ô�µ�
		this->TradeLong(pDepthMarketData);
		this->TradeShort(pDepthMarketData);
	}
	//int result = this->tdapi->ReqQryOrder(pOrder, nRequestID);
}
void  StrategyFICCW::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void  StrategyFICCW::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

}
void StrategyFICCW::OnInit()
{
	shared_ptr<byte> lasttradingtime = this->parampersistencer->GetObjectParameter("lasttradingtime", sizeof(time_t));
	memcpy(&lasttradingtime, lasttradingtime.get(), sizeof(time_t));

}

void StrategyFICCW::OnRspQryOrder(CThostFtdcOrderField *pOrder, int nRequestID, bool bIsLast)
{

}

void StrategyFICCW::OnRspQryTrade(CThostFtdcTradeField *pTrade, int nRequestID, bool bIsLast)
{

}

void StrategyFICCW::OnTime(tm t)
{
	// ������һ��30 ÿ��5����ȥ��ȡһ�� ��ʼ�����ļ���������������������ô
	//if (needtradelong==false&&(t.tm_hour >= 9&&t.tm_hour<15)&&t.tm_min==5&&t.tm_sec==0) { //5



	if (((this->lasttradingtime>0 && mktime(&t)>this->lasttradingtime + 60 * 60 * 24 * 5)) || this->lasttradingtime == 0)
		this->PyRun();
	if (needtradelong == false && (t.tm_hour >= 9 && t.tm_hour<15)) { //5

		needtradelong = this->IsNeedTradeLong();

		memcpy(lastradelongday, this->TradingDay, sizeof(lastradelongday));
	}
	if (needtradelong == true) {

		if (strcmp(lastradelongday, this->lastradelongday) != 0)//���tradingday �Ѿ�����ͬ����ô��Ҫ��������ȡ����
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

		if (strcmp(lastradeshortday, this->lastradeshortday) != 0)//���tradingday �Ѿ�����ͬ����ô��Ҫ��������ȡ����
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


void StrategyFICCW::TradeLong(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if ((needtradelong&&hascompletetradelong) || needtradelong == false)
		return;


	bool tempneedtrade = false;

	for (string instrumentid : list_longpositon) { //ȥ��������
		string ctpinstrumentid = instrumentid.substr(0, instrumentid.find('.'));
		if (strcmp(ctpinstrumentid.c_str(), pDepthMarketData->InstrumentID) != 0)
		{
			tempneedtrade = true;
			continue;
		}
		CThostFtdcInstrumentField  instrument = this->instrumentmap->getInstrumentid(ctpinstrumentid);
		double multi = instrument.VolumeMultiple; //��Լ����
		double marginratio = instrument.LongMarginRatio;  //��ͷ��֤����

		int volume = (this->singglemount / (pDepthMarketData->LastPrice*multi*marginratio));

		if (this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Buy)<volume && this->portfoliomanager->GetOrderInputCount(ctpinstrumentid) == 0 && this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Buy) == 0)
		{

			//���ʱ����2:50֮ǰ ��ô

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
				//ƽ���õ���	

				int volume = this->portfoliomanager->GetPostionDetailVolume(detail.InstrumentID, THOST_FTDC_D_Buy);
				CThostFtdcInputOrderField orderinput = this->CreateOrder(detail.InstrumentID, pDepthMarketData->LowerLimitPrice, this->portfoliomanager->GetPostionDetailVolume(detail.InstrumentID, THOST_FTDC_D_Buy), THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_CloseYesterday);
				int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);


				tempneedtrade = true;



			}
			break;
		}

	}






}
void StrategyFICCW::TradeShort(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if ((needtradeshort&&hascompletetradeshort) || needtradeshort == false)
		return;

	bool tempneedtrade = false;

	for (string instrumentid : list_shortpositon) { //ȥ��������
		string ctpinstrumentid = instrumentid.substr(0, instrumentid.find('.'));
		if (strcmp(ctpinstrumentid.c_str(), pDepthMarketData->InstrumentID) != 0)
			continue;
		CThostFtdcInstrumentField  instrument = this->instrumentmap->getInstrumentid(ctpinstrumentid);
		double multi = instrument.VolumeMultiple; //��Լ����
		double marginratio = instrument.ShortMarginRatio;  //��ͷ��֤����

		int volume = (this->singglemount / (pDepthMarketData->LastPrice*multi*marginratio));

		this->logger->Log("%s volume %d \n", ctpinstrumentid.c_str(), volume);
		this->logger->Log("%s postion %d \n", ctpinstrumentid.c_str(), this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell));
		this->logger->Log("%s orderinputcount %d \n", ctpinstrumentid.c_str(), this->portfoliomanager->GetOrderInputCount(ctpinstrumentid));
		this->logger->Log("%s ordercount %d \n", ctpinstrumentid.c_str(), this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Sell));
		if (this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell)<volume && this->portfoliomanager->GetOrderInputCount(ctpinstrumentid) == 0 && this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Sell) == 0)
		{

			//���ʱ����2:50֮ǰ ��ô

			CThostFtdcInputOrderField orderinput = this->CreateOrder((char*)ctpinstrumentid.c_str(), pDepthMarketData->LowerLimitPrice, volume - this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell), THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);
			int result = this->tdapi->ReqOrderInsert(&orderinput, this->requestid++);
			tempneedtrade = true;

		}
		if (this->portfoliomanager->GetPostionDetailVolume(ctpinstrumentid, THOST_FTDC_D_Sell)>volume && this->portfoliomanager->GetOrderInputCount(ctpinstrumentid) == 0 && this->portfoliomanager->GetOrderCount(ctpinstrumentid, THOST_FTDC_D_Buy) == 0)
		{

			//���ʱ����2:50֮ǰ ��ô
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
				//ƽ���õ���			
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
vector<string> StrategyFICCW::GetPostionListFromFile(string files)
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
bool StrategyFICCW::IsNeedTradeLong()
{
	time_t tm_list;
	time_t tm_now;
	string lastdate;
	string longfile = "long_w.csv";

	list_longpositon = this->GetPostionListFromFile(longfile);
	lastdate = list_longpositon[0];
	tm_list = StringToDatetime(list_longpositon[0]);
	tm_now = this->timer->GetTCurrentTime();
	list_longpositon.erase(list_longpositon.begin()); //ɾ�����ڡ�
	double ms = difftime(tm_now, tm_list);
	double days = ceil(ms / 86400);
	if (days <= 2)
		return true;
	else
		return false;
}



bool StrategyFICCW::IsNeedTradeShort()
{

	time_t tm_list;
	time_t tm_now;
	string lastdate;
	string shortfile = "short_w.csv";
	list_shortpositon = this->GetPostionListFromFile(shortfile);
	lastdate = list_shortpositon[0];
	tm_list = StringToDatetime(list_shortpositon[0]);
	tm_now = this->timer->GetTCurrentTime();
	list_shortpositon.erase(list_shortpositon.begin()); //ɾ�����ڡ�
	double ms = difftime(tm_now, tm_list);
	double days = ceil(ms / 86400);
	if (days <= 2)
		return true;
	else
		return false;



}
//�ǲ����ڶ࿪�б�
bool StrategyFICCW::IsInLongList(string instrumentid)
{
	/*vecotr<> list_postion=*/
	bool rvalue = false;
	for (string pinstrumentid : list_longpositon) {
		if (strstr(pinstrumentid.c_str(), instrumentid.c_str()) != 0) {//��Ϊ����
			rvalue = true;
			break;

		}

	}
	return rvalue;

}
bool StrategyFICCW::IsInShortList(string instrumentid)

{
	bool rvalue = false;
	for (string pinstrumentid : list_shortpositon) {

		if (strstr(pinstrumentid.c_str(), instrumentid.c_str()) != 0) {

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
time_t StrategyFICCW::StringToDatetime(string str)
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
void StrategyFICCW::PyRun()
{
	fstream file;
	file.open("python/SimTrade_Wai.py", ios::in);
	if (file)
	{
		system("python python/SimTrade_Wai.py");
	}
}