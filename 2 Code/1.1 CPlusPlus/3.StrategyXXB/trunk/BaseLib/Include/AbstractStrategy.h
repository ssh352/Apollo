#pragma once
#include <string>
#include <ThostFtdcMdApi.h>
#include <ThostFtdcTraderApi.h>
#include <ThostFtdcUserApiStruct.h>
#include "AbstractTdApi.h"
#include <vector>
#include <deque>
#include<map>
#include <thread>
#include<afxwin.h>
#include "AbstractPortfolioManager.h"
#include "AbstractParameterPersistencer.h"
#include "AbstractInstrumentMap.h"
#include "AbstractInstrumentCommissionRateMap.h"
#include "AbstractMDDB.h"
#include "AbstractTimer.h"
#include "LOGGER.h"
#include <memory>
#include <atomic>
#include "TdApiWrapper.h"
using namespace Abstract;
//#include "SimulateServer.h"
using namespace std;
using namespace CTPTOOL;

#define STR(s) #s
namespace Trade {
	class TradeStrategyManager;
	class TradeCommandLine;
}
namespace Simulate {
	class SimulateServer;
	class SimulateStrategyManager;
};
namespace Abstract {
	class MTOrderExcuter;
	class OrderExcuter;

}
using namespace Trade;
using namespace Simulate;
using namespace Abstract;
namespace Abstract {
	BASELIB_API enum StrategyStatus{
		uninit = 1,
		running=2,
		initing=3,
		initted=4,
		stop=5,
		stoped=6,
		unloading = 7	    
	};
	 class BASELIB_API AbstractStrategy
	{
	public:
		
		friend class Simulate::SimulateServer;//��Ԫ
		friend class TdApiWrapper;
		friend class MTOrderExcuter;
		friend class TradeStrategyManager;
		friend class SimulateStrategyManager;
		friend class TradeCommandLine;
		friend class OrderExcuter;
		AbstractStrategy();
		virtual ~AbstractStrategy();
		virtual boolean IsInstanceOf(string classname);
		virtual string GetStrategyID() final; //��ò��Ե�id
		virtual string getStrategyName() final;
		virtual void setTdApi(AbstractTdApi * tdapi) final;
		virtual void setMdApi(CThostFtdcMdApi *mdapi) final;		
		StrategyStatus AbstractStrategy::getStatus();
		virtual void setStatus (StrategyStatus status)final;
		virtual void setBrokerid(string brokerid )final; 
		virtual void setInvestid(string investid)final;
		virtual vector<string> getSubscribe() final;
		virtual void setFrontid(int frontid) final;
		virtual void setSessionid(int sessionid) final;
		virtual void setTradingDay(TThostFtdcDateType	TradingDay) final;
		virtual string GetDllPath() final;
		virtual void SetDllPath(string dllpath) final;
		virtual void SetPortfolioManager(shared_ptr<AbstractPortfolioManager>  portfoliomanager);
		virtual void SetParameterPersistencer(shared_ptr<AbstractParameterPersistencer>  parameterpersistencer);
		virtual void SetInstrumentMap(shared_ptr<AbstactInstrumentMap> instrumentmap);
		virtual void SetInstrumentComissionRateMap(shared_ptr<AbstractInstrumentCommissionRateMap> instrumentcommissionratemap);
		virtual void SetLogger(shared_ptr<LOGGER> logger);
		virtual void SetMDDB(shared_ptr<AbstractMDDB> mddb);
		virtual void SetParameter(map<string,double> param);//���ò���
		virtual void SetTimer(shared_ptr<AbstractTimer> timer);
		virtual map<string, pair<double, double>> GetParameterArea();//��ȡ�����Ż��ռ�
		boolean HasInit();
		
		void Init() ;	
	public:
		vector<string>  GetPara();		
		shared_ptr<AbstractPortfolioManager>  GetPortfolioManager();
		map<string, double> GetIndicators();//��ü�ʱ��ָ�� 
		typedef std::function<void(TThostFtdcDateType	TradingDay)> Pf_OnSetTradingDay;
		Pf_OnSetTradingDay pf_onsettradingday;
	protected:

	virtual	void OnCommand(string path, string command, string parameter); //������Ӧ�����еĺ���
	private:
	
		void  putMarketData(CThostFtdcDepthMarketDataField * marketdata);//�ⲿ�����������ɴ��ɷ�
		void  putTimeData(tm t);
		void  putCommand(string path, string command, string parameter);
		void  RspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);// ���еĲ�������Ӧ�ɴ��ɷ�
		void  RtnOrder(CThostFtdcOrderField *pOrder);
		void  RtnTrade(CThostFtdcTradeField *pTrade); //�ɽ��ر��ɴ��ɷ�	
		void  RspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void  OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, int nRequestID, bool bIsLast); //�ؽ��ֲֶ���
		void  OnRspQryOrder(CThostFtdcOrderField *pOrder, int nRequestID, bool bIsLast);//���عҵ��ĵ���
		void  OnRspQryTrade(CThostFtdcTradeField *pTrade, int nRequestID, bool bIsLast);//���سɽ��ĵ���
	
		void SetIsSimulate(bool issimulate); //�Ƿ�Ϊsimulate ����Ǿʹ���true ������false
		bool  IsEventlistEmpty();//�������Ϣ�����Ƿ�Ϊ�� ��Ҫ����ģ�⻷�� ʹ��ģ����������������ͬ����
		thread* t; //��̨�߳�
	protected:
		CRITICAL_SECTION cm;
		vector<string> inheritchain; //�̳��� �����಻�������ؼ̳�
		bool issmiulate;   //�Ƿ���ģ�⻷��


	private://Ϊ�˽����ֳɽ�����ر����л��Ļ������
	
		deque<CThostFtdcTradeField> tradeorderlist; //�ɽ��ر�����
		deque<CThostFtdcOrderField>  orderlist;    //�ҵ��ر�����
		deque<CThostFtdcDepthMarketDataField> marketdatalist;
		deque<tm> timelist;//ʱ����Ϣ
	
		//--
		StrategyStatus backthreadstatus;//��̨�߳��Ƿ��Ѿ�����	    
		void eventloop();
		void start();
		static atomic<int> orderref;
	    protected:
		   virtual void OnRtnTrade(CThostFtdcTradeField *pTrade) = 0; //�ɽ��ر�		
		   virtual void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
		   virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) = 0; //������պ���
		   virtual void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) = 0;
		   virtual void OnRtnOrder(CThostFtdcOrderField *pOrder) = 0;
		   virtual  void OnTime(tm t) = 0;
		   virtual void OnInit() = 0;
		   virtual void OnReset();//����
		   /* ���������Ҫ���� ���׵���Ʒ��ʱ�� */
	/*	   CThostFtdcInputOrderField CreateOrder(char* instrumentid, double price, int volume, char direction, char orderpricetype, char timecondition);*/

		   //�Ƿ񿪲ֵ�
		   CThostFtdcInputOrderField CreateOrder(char* instrumentid, double price, int volume, TThostFtdcDirectionType direction, TThostFtdcOrderPriceTypeType orderpricetype, TThostFtdcTimeConditionType timecondition, char CombOffsetFlag);
		   void AutoCloseOrder(char* instrumentid, double price, TThostFtdcDirectionType direction, TThostFtdcOrderPriceTypeType orderpricetype, TThostFtdcTimeConditionType timecondition);
		   StrategyStatus status;
		   string brokerid;
		   string investid;
		   string strategyname;
		   string author;//����
		   string version;//�汾
		   string dllpath;//���ص�·��
		   string desc; //��������
		   TdApiWrapper* tdapi;
		   CThostFtdcMdApi *mdapi;
		   int frontid;
		   int sessionid;
		   bool hasinit;  //�Ƿ��Ѿ���ʼ��
		   TThostFtdcDateType	TradingDay;
		   vector<string> paralist;
		   vector<string> subscribelist; //�����б�
		   virtual string GetOrderRef() final;        //�õ���������
		   virtual string GetNewOrderRef() final;  //�õ��µı�������
		   int strategyid;	
		   static atomic<int>  requestid;
		   map<string, double> indicators;//ָ��
		   shared_ptr<AbstractPortfolioManager>  portfoliomanager;
		   shared_ptr<AbstractParameterPersistencer> parampersistencer; //�־û�
		   shared_ptr<AbstactInstrumentMap> instrumentmap;//Ʒ����Ϣ
		   shared_ptr<AbstractInstrumentCommissionRateMap> instrumentcommissionmap;//������Ϣ
		   shared_ptr<LOGGER> logger;//��־
		   shared_ptr<AbstractTimer> timer;
		   shared_ptr<AbstractMDDB> mddb; //�������ݿ�

	};
}