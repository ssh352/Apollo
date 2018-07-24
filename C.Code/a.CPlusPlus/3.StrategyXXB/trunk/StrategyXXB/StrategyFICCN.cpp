#include "stdafx.h"
#include "StrategyFICCN.h"
#include <iostream>  
#include <math.h>
#include <string> 
#include <time.h>
#include <vector>  
#include <fstream>  
#include <sstream>  
#include <set>
using namespace std;
using namespace Abstract;
//#define MOM //��������̷�ص�mom ��Ҫ��������� ��Ȼ�͹ر������
//#define  CHANGEMULTI  //�޸ı�����ϵ  

StrategyFICCN::StrategyFICCN()
{
	this->strategyid = 6002;
	this->strategyname = "չ��ִ�в���";
	this->version = "0.1";
	this->desc = "����ִ��չ�ڲ����µĵ�";
	this->author = "���װ�";	
    #define mul 3  //����
	this->singglemount = 300000*mul;//
	this->lasttradingtime = 0;
	this->orderexcuter = new OrderExcuter();
	this->orderexcuter->SetStrategy(this);
	this->orderexcuter->SetIsExcute(false);
	this->lasttickmap = new LastTickMap();
}


StrategyFICCN::~StrategyFICCN()
{
	delete this->orderexcuter;
	delete this->lasttickmap;
}

void StrategyFICCN::OnRtnTrade(CThostFtdcTradeField *pTrade)
{

	this->logger->Log("�ɽ�  %s %d ",pTrade->InstrumentID,pTrade->Volume);


}
void StrategyFICCN::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}
void StrategyFICCN::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
	this->lasttickmap->OnRtnDepthMarketData(*pDepthMarketData);
	this->orderexcuter->OnDepathMarket(pDepthMarketData);
	//int result = this->tdapi->ReqQryOrder(pOrder, nRequestID);
}
void  StrategyFICCN::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//this->logger->Log("���������쳣");
}
void  StrategyFICCN::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

}
void StrategyFICCN::OnInit()
{
	shared_ptr<byte> lasttradingtime1 = this->parampersistencer->GetObjectParameter("lasttradingtime", sizeof(time_t));
	memcpy(&lasttradingtime, lasttradingtime1.get(), sizeof(time_t));
 
}

void StrategyFICCN::OnRspQryOrder(CThostFtdcOrderField *pOrder, int nRequestID, bool bIsLast)
{

}

void StrategyFICCN::OnRspQryTrade(CThostFtdcTradeField *pTrade, int nRequestID, bool bIsLast)
{
	
}

void StrategyFICCN::OnTime(tm t)
{
// ������һ��30 ÿ��5����ȥ��ȡһ�� ��ʼ�����ļ���������������������ô
	//if (needtradelong==false&&(t.tm_hour >= 9&&t.tm_hour<15)&&t.tm_min==5&&t.tm_sec==0) { //5

	

	if (((this->lasttradingtime > 0 && mktime(&t) > this->lasttradingtime + 60 * 60 * 20 )&&t.tm_hour>9) || this->lasttradingtime == 0) {
		//this->PyRun();
		this->logger->Log("��");
		vec_longpostion = GetPostionListFromFile("long.csv");
		this->logger->Log("��");
		vec_shortpostion = GetPostionListFromFile("short.csv");
		for (auto &instrumentid : vec_longpostion)   //vector ת��set
		{
			set_longpostion.insert(instrumentid);
		}
		for (auto &instrumentid:vec_shortpostion)
		{
			set_shortpostion.insert(instrumentid);
		}
		this->lasttradingtime = this->timer->GetTCurrentTime();
		shared_ptr<byte> b = shared_ptr<byte>(new byte[sizeof(time_t)], [](byte *p) {delete[] p; });
		memcpy(b.get(), &lasttradingtime, sizeof(time_t));
		 this->parampersistencer->AddOrUpdataParameter("lasttradingtime", b,sizeof(time_t));
	}



	if (vec_longpostion.size() > 0||vec_shortpostion.size()>0)
	{
		this->orderexcuter->SetIsExcute(false);
		CThostFtdcInputOrderField orderinput;
		//�ж��Ƿ�׼���������еļ۸�����
		set<string> set_instrumentid = this->portfoliomanager->GetInstrumentIdListHasPostion();
		if (this->lasttickmap->IsAllTickReady(vec_longpostion) && this->lasttickmap->IsAllTickReady(vec_shortpostion)&&this->lasttickmap->IsAllTickReady(set_instrumentid)) { //�ڼ۸����ݶ�׼���õ���������������б�
			//�õ�Ŀǰ�Ķ�� ����Ŀǰ�Ķ���Ƿ��� ��Ҫ�൥���б�֮�� ���������ôƽ��
			set<string> set_currentlongpostion = this->portfoliomanager->GetInstrumentIdListHasPostion(THOST_FTDC_D_Buy);
			for (string strinstrument : set_currentlongpostion) {
				if (set_longpostion.count(strinstrument) == 0) {  //˵���óֲֺ�Լ����Ҫƽ����
					int volume = this->portfoliomanager->GetPostionDetailVolume(strinstrument,THOST_FTDC_D_Buy);
					CThostFtdcDepthMarketDataField depthmarketdatafiled = this->lasttickmap->getLastTick(strinstrument);
				CThostFtdcInstrumentField  instrument = this->instrumentmap->getInstrumentid(strinstrument);
    #ifdef MOM
					 orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice, volume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Close);
#else
					 orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice, volume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_CloseYesterday);
#endif
					this->orderexcuter->AddOrder(orderinput);
				}
			}
			//�õ�Ŀǰ�Ŀղ� ����Ŀǰ�Ŀղ��Ƿ��� ��Ҫ�ղֵ��б�֮�� ���������ôƽ��
			set<string> set_currentshortpostion = this->portfoliomanager->GetInstrumentIdListHasPostion(THOST_FTDC_D_Sell);
			for (string strinstrument : set_currentshortpostion) {
				int volume = this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell);
				if (set_shortpostion.count(strinstrument) == 0) {  //˵���óֲֺ�Լ����Ҫƽ����              
				CThostFtdcDepthMarketDataField depthmarketdatafiled = this->lasttickmap->getLastTick(strinstrument);
				CThostFtdcInstrumentField  instrument = this->instrumentmap->getInstrumentid(strinstrument);
               #ifdef MOM
				 orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.UpperLimitPrice, volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Close);
               #else
				 orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.UpperLimitPrice, volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_CloseYesterday);
               #endif
					this->orderexcuter->AddOrder(orderinput);
				}

			}
		  
			for (string strinstrument : vec_longpostion) {

				CThostFtdcInstrumentField  instrument = this->instrumentmap->getInstrumentid(strinstrument);
				double multi = instrument.VolumeMultiple; //��Լ����
				double marginratio = instrument.LongMarginRatio;  //��ͷ��֤����
				CThostFtdcDepthMarketDataField depthmarketdatafiled=this->lasttickmap->getLastTick(strinstrument);
				int volume = (this->singglemount / (this->lasttickmap->getLastTick(strinstrument).LastPrice*multi));
              #ifdef  CHANGEMULTI
				if ((volume - this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy) >0) ) { //��Ҫ����

					orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.UpperLimitPrice, volume - this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy), THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);
					this->orderexcuter->AddOrder(orderinput);
				}
               #else

				if ((volume-this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy) >0)&& this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy)==0) { //��Ҫ����

					 orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.UpperLimitPrice, volume - this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy), THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);
					this->orderexcuter->AddOrder(orderinput);
				}
              #endif

				//else if(volume - this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy)<0) { //��Ҫƽ������Ĳ���
    //          #ifdef MOM
				//	orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice, this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy) - volume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Close);
    //          #else
				//	 orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice, this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Buy)-volume, THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_CloseYesterday);
    //          #endif
				//	this->orderexcuter->AddOrder(orderinput);
				//}
			
			}

			//����ղ�    
		for (string strinstrument : vec_shortpostion) {

				CThostFtdcInstrumentField  instrument = this->instrumentmap->getInstrumentid(strinstrument);
				double multi = instrument.VolumeMultiple; //��Լ����
				double marginratio = instrument.ShortMarginRatio;  //��ͷ��֤����
				CThostFtdcDepthMarketDataField depthmarketdatafiled = this->lasttickmap->getLastTick(strinstrument);
				int volume = (this->singglemount / (this->lasttickmap->getLastTick(strinstrument).LastPrice*multi));

          #ifdef  CHANGEMULTI
				if (volume-this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell)>0) {  //��Ҫ�࿪һ��
					 orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice, volume- this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell), THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);
					this->orderexcuter->AddOrder(orderinput);
				}
          #else
				if (volume - this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell)>0 && this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell) == 0) {  //��Ҫ�࿪һ��
					orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice, volume - this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell), THOST_FTDC_D_Sell, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Open);
					this->orderexcuter->AddOrder(orderinput);
				}
          #endif


			//else if (volume-this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell)  < 0) //��Ҫƽ��һ��
			//	{
   //        #ifdef MOM
			//	orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice,  this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell)-volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_Close);
   //         #else
			//	orderinput = this->CreateOrder((char*)strinstrument.c_str(), depthmarketdatafiled.LowerLimitPrice, this->portfoliomanager->GetPostionDetailVolume(strinstrument, THOST_FTDC_D_Sell) - volume, THOST_FTDC_D_Buy, THOST_FTDC_OPT_LimitPrice, THOST_FTDC_TC_GFD, THOST_FTDC_OFEN_CloseYesterday);
   //         #endif
			//	this->orderexcuter->AddOrder(orderinput);

			//	}
				
			}
		string tempstr = "����ִ���б�����Ѿ������ļ�order" + this->timer->GetStrCurrentTime("%Y-%m-%d") + ".txt";

		this->logger->Log(tempstr.c_str());
		this->orderexcuter->PrintOrder("order"+this->timer->GetStrCurrentTime("%Y-%m-%d")+".txt");

		this->logger->Log("ȷ�Ϻ�����������excuteorder �µ�");
	
		vec_longpostion.clear(); //���
		vec_shortpostion.clear(); //���
		set_longpostion.clear();
		set_shortpostion.clear();

		}
		//this->orderexcuter->WriteToFile("");
		

	}


	//if(t.tm_hour>=15&&t.tm_min==5)

}





/****************************************************************
Date: 2017-11-06
Author: xia xianba
Copyright: HeXin Invest Inc.
Description: 
  - Input: The CSV file containing transaction information
  - Output: A vector contains csv file after parsing
****************************************************************/
vector<string> StrategyFICCN::GetPostionListFromFile(string files)
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
	{
		str = str.substr(0, str.find('.'));
		lineArray.push_back(str);
	}
	inFile.close();
	
	for (int i = 0; i < lineArray.size(); i++)
		this->logger->Log("LineArray[%d]: %s. \n", i, lineArray[i].c_str());
	lineArray.erase(lineArray.begin());
	return lineArray;
}



/****************************************************************
Date: 2017-11-21
Author: xia xianba
Copyright: HeXin Invest Inc.
Description: Run Python Program
****************************************************************/
void StrategyFICCN::PyRun()
{
	fstream file;
	file.open("python/SimTrade.py", ios::in);
	if(file)
	{
		system("python python/SimTrade.py");
	}
}

void StrategyFICCN::OnCommand(string path, string command, string parameter)
{   
	if (strcmp("ls", command.c_str())==0) {
		this->logger->Log("readfromfile:ֱ�Ӵ��ļ����ȡ��λ��ִ��\n");
		this->logger->Log("excutepython:��������python\n");
		this->logger->Log("excuteorder:ִ���µ�\n");
		this->logger->Log("setmulti");
	}
	if (strcmp("readfromfile", command.c_str())==0)
	{
		//
		this->logger->Log("��");
		vec_longpostion = GetPostionListFromFile("long.csv");
		this->logger->Log("��");
		vec_shortpostion = GetPostionListFromFile("short.csv");


		for (auto &instrumentid : vec_longpostion)   //vector ת��set
		{
			set_longpostion.insert(instrumentid);

		}
		for (auto &instrumentid : vec_shortpostion)
		{
			set_shortpostion.insert(instrumentid);

		}
		//
		//this->orderexcuter->T

	}
	if (strcmp("excutepython", command.c_str())==0) {


		this->PyRun();

	  }
	if (strcmp("excuteorder", command.c_str())==0) {

		this->orderexcuter->SetIsExcute(true);
	}



}
