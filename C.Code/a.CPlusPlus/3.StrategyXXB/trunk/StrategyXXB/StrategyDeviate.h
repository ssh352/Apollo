#pragma once
#include "IndicatorDataType.h"
#include <vector>
#include "ThostFtdcUserApiStruct.h"
#include <map>
#include "AbstractStrategy.h"
#include "mmsystem.h"
using namespace Abstract;
using namespace Indicator;
using namespace std;
class StrategyDeviate :public AbstractStrategy
{
public:
	StrategyDeviate();
	~StrategyDeviate();
	//测试用变量
	bool bTest;
	int iTestTime;
	int iCountTime;
	int iOrderNum;
	double dTotalMoney;                 //初始金额；
	int  iPosition;                     //仓位；
	double m_dBailRatio;                  //保证金比例；
	double m_dWinStop;                             //止盈；
	double m_dLossStop;                            //止损；
	double m_dDeviateRatio;                        //偏离程度；
	double m_dAddRatio;
	bool bCloseAll = false;     //现价平仓所有持仓单；
	bool bOneTime;

	bool bTimeControlDown2Up = false;     //时间控制；9：00—9：30不开仓；14：28以后不开仓； 在从低往上的时候，如果高开2%,不受9：00到9：30的限制；如果低开2%,则不受限制；  
	int iOpenTime = 0;
	bool bTime1015 = false;
	bool bTime1130 = false;

	void ReadFile(CString str);
	void ReadCSVFile(CString str);
	void GetPara();
	vector<string> m_vecPara;
	vector<string> m_listInstructmentID;
	vector<CThostFtdcInvestorPositionDetailField> m_listPosition;
	vector<CThostFtdcOrderField> m_listOrder;
	string strategyid_str;
	tm now;
	struct CRecordData
	{
		bool m_bOpen;                  //判断是否当日开仓；
		CTime m_tLastOpen;             //记录上次开仓时刻；
		double m_dOpenPrice;           //开仓下单价格；
		double m_dPrice;               //开仓时刻的现价；
		double m_dRasingLimit;         //获得该品种的涨停价格；
		bool bOneTime;                 //运行一次标记；
		bool bHasOrder;                //是否有订单；
		double m_dPrice2T;             //两跳的价格；
		int iDirection;                //下单方向；-1为Sell,+1为Buy,0为初始值；
		CString m_strOrderRef;
		double m_dPrice1T;             //1跳的价格；
		int m_iLever;                  //品种杠杆倍数；
		int m_iShQS;                   //判断是否属于上期所品种；0表示不是，1表示是；
		double m_dDown2UpRange;        //由下往上的幅度；
		double m_dDown2UpAddRatio;     //做多加仓幅度；
		double m_dDown2UpWinStop;
		double m_dUp2DownRange;        //由上往下的幅度；
		double m_dUp2DownAddRatio;     //由上往下的加仓幅度；
		double m_dUp2DownWinStop;      //
		double m_dPreSettlement;       //昨天的结算价；   
		bool bStop;                    //当天永久停止不开仓；
		CTime m_tUpperTime;            //记录涨停时间；
		CTime m_tLowerTime;            //记录跌停时间；
		double m_dTrustRatio;          //保证金比例；
		int m_iAddTimes;               //加仓次数；每个品种加仓次数不得大于2；
		bool bOpenHigh;                //是否高开
		bool bOpenLow;				   //是否低开
		double m_dAlarmRatio;          //报警偏离度；
		double m_dSafeRatio;           //安全垫值；

		int iCountLimitTimes;
		vector<CBars> m_Vec1MiniuteBars;
		vector<CThostFtdcDepthMarketDataField> vectickTmpData;
		bool m_bTickStart;
		int m_iLastMinute;
		int m_iLastSecond;
		double m_dLastVolume;
		CBars tmpBar;
		bool m_bFlag;
		CTime m_LastTime;   //上一次Tick的时间；
		bool bSpecialFlag;
		int iCountSpecial;
	};
	CTime GetTime(string strDate, string strTime);
	map<string, CRecordData>   m_MapRecordData;   //主力合约当天成交记录；
	map<string, CThostFtdcDepthMarketDataField> m_MapTick;
	time_t GetTimeStamp(string date, string time);
	CThostFtdcInputOrderActionField MakeCancelOrder(CThostFtdcOrderField order);
	map<string, time_t> m_MapOrderInput;
protected:
	bool ConditionMeanPriceDown(double dLastMeanPrice, double dNowMeanPrice, double dRatio);
	bool ConditionOpenHigh(double dLastMeanPrice, double dNowMeanPrice, double dRatio);
	bool TimeControl(CTime tTime);
	void Deviate(CThostFtdcDepthMarketDataField *pDepthMarketData, double dDeviateRatio, double dSettlementPrice);
	void OutPutDeviateData(string strInstrumentID, string strUpdateTime, double dSettlementPrice, double dLastPrice, double dDeviateRatio, double dThresld);
	CThostFtdcInputOrderField MakeOrder(char* instrumentid, double price, int volume, char direction, char ordertype, char timecondition);
	virtual void OnRtnTrade(CThostFtdcTradeField *pTrade); //成交回报		
	virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //行情接收函数
	virtual void  OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void  OnRtnOrder(CThostFtdcOrderField *pOrder);
	virtual void OnInit();
	virtual void OnTime(tm t);
};

