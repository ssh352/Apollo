#pragma once
#include <vector>
#include <set>
#include "ThostFtdcUserApiStruct.h"
#include "AbstractInstrumentMap.h"
#include <memory>
#include<afxwin.h>
using namespace std;
using namespace Abstract;

// ��Ϲ��� ����ÿ�����Ե� �ֽ������ѡ����ᱣ֤�𡡲�λ��  ���������������Ե�Ҳ�������������˻���
namespace Abstract {

	class AbstractStrategy;
}
namespace Trade {
	class TradeStrategyManager;
}
namespace Abstract {
	 class BASELIB_API AbstractPortfolioManager
	{
	public:
		AbstractPortfolioManager();
		virtual ~AbstractPortfolioManager();
		vector<CThostFtdcTradeField> tradedorderlist; //ÿ�ɽ�һ�����Ӿ�����������һ��
		vector<CThostFtdcOrderField>  orderlist;    //�ҵ�����  
		vector<CThostFtdcInputOrderField> orderinputlist;//�ҵ�������� ���Ӳ���ɹ�����orderlist������һ������orderinputlist����orderinput ɾ����
		vector<CThostFtdcInputOrderActionField> reqactionlist;//�����������
		vector<CThostFtdcInvestorPositionField> inverstpositionlist;//�ֲֶ���
	    vector<CThostFtdcInvestorPositionDetailField> inverstpositiondetaillist;//�ֲֶ�����ϸ��
		CThostFtdcTradingAccountField accountstatus;
		shared_ptr<AbstactInstrumentMap> instrumentmap;
		
	private:
		friend class Abstract::AbstractStrategy; //��Ԫ
		friend class Trade::TradeStrategyManager;
		void OnRtnTrade(CThostFtdcTradeField *pTrade); //�ɽ��ر�		
		void OnRtnOrder(CThostFtdcOrderField *pOrder);
		void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData); //������պ���
		void OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
		void OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//
		void OnRspQryInvestorPosition(CThostFtdcInvestorPositionField *pInvestorPosition, int nRequestID, bool bIsLast); //�ؽ��ֲֶ���
		TThostFtdcDateType	TradingDay;
		void OnRspQryOrder(CThostFtdcOrderField *pOrder, int nRequestID, bool bIsLast);
		void OnRspQryTrade(CThostFtdcTradeField *pTrade, int nRequestID, bool bIsLast);
		int lastpostionnRequestID;// ���һ���ؽ��ֲֶ��е�nrequestid
		int lasttradenRequestID;  //���һ���ؽ��ֳɽ����е�nrequestid
		int lastordernRequestID;  //���һ�β�ѯ�ؽ��ҵ�״̬��nrequestid
	protected:
		int strategyid; //���Ե�id 
		double cash;  //�ֽ𾻶�

		CRITICAL_SECTION cm;  //������ ��ط�����Ҫ�ĳɱ�׼c++ ��mux 
		/*�����ʲ�����*/
		void virtual LoadPortfolioManager()=0;//
		/*�洢һ������*/
		void virtual StoreTrade(int strategyid, CThostFtdcTradeField trade)=0;
		/*ɾ��һ������*/
		void virtual DeleteStoreTrade(int strategyid, CThostFtdcTradeField trade) = 0;
		/*�洢һ�����׵�ED��*/
		void virtual StoreTradeED(int strategyid, CThostFtdcTradeField trade) = 0;

		/**/
		void virtual StoreInvestorPositionDetail(int strategyid, CThostFtdcInvestorPositionDetailField positiondetail)=0;
		/*�޸�һ����û��ƽ�ĳֲ�*/
		void virtual UpdateStoreInvestorPositionDetail(int strategyid, CThostFtdcInvestorPositionDetailField positiondetail) = 0;
		/*ɾ��һ��*/
		void virtual DeleteStoreInvestorPositionDetail(int strategyid, CThostFtdcInvestorPositionDetailField positiondetail) = 0;
		void virtual StoreInvestorPositionDetailED(int strategyid, CThostFtdcInvestorPositionDetailField positiondetail, TThostFtdcDateType closedate, TThostFtdcDateType closetradingday, TThostFtdcTimeType closetime, double closeprice) = 0;

	public:
		void SetInStrumentMap(shared_ptr<AbstactInstrumentMap> instrumentmap);
		virtual void setTradingDay(TThostFtdcDateType	TradingDay) final;
		/*���ò���id*/
		void SetStrategyID(int strategyid);
		/*�õ����йҵ�������*/
		int GetOrderCount(); //
		/*����instrumentid�ҵ�������*/
		int GetOrderCount(string instrumentid);  
		/*����instrumentid�õ��ҵ�������
		  @instrumentid 
		  @direction ��շ���
		*/
		int GetOrderCount(string instrumentid, TThostFtdcDirectionType direction); //
		/*����instrumentid ���� ��ƽ ��ùҵ�������
		@instrumentid
		@direction ��շ���
		@offset ��ƽ
		*/
		
		int GetOrderCount(string instrumentid, TThostFtdcDirectionType direction,TThostFtdcOffsetFlagType offset); //

		int GetOrderCount(string instrumentid, TThostFtdcDirectionType direction, TThostFtdcOffsetFlagType offset, double price);

		int GetUnTradeOrderVolume(); //
							 /*����instrumentid�ҵ�������*/
		int GetUnTradeOrderVolume(string instrumentid);
		/*����instrumentid�õ��ҵ�������
		@instrumentid
		@direction ��շ���
		*/
		int GetUnTradeOrderVolume(string instrumentid, TThostFtdcDirectionType direction); //
																				   /*����instrumentid ���� ��ƽ ��ùҵ�������
																				   @instrumentid
																				   @direction ��շ���
																				   @offset ��ƽ
																				   */

		int GetUnTradeOrderVolume(string instrumentid, TThostFtdcDirectionType direction, TThostFtdcOffsetFlagType offset); //

		int GetUnTradeOrderVolume(string instrumentid, TThostFtdcDirectionType direction, TThostFtdcOffsetFlagType offset, double price);

		vector<CThostFtdcOrderField> GetOrder(string instrumentid, TThostFtdcDirectionType direction, TThostFtdcOffsetFlagType offset);
		/*
		��������Ч�Ĺҵ�����
	
		*/
		vector<CThostFtdcOrderField> GetOrder();
		/*����OrderRef ��ùҵ�����
		@ref OrderRef
		*/
		vector<CThostFtdcOrderField> GetOrderFormStrategy(string ref);
		

		/*
		�����Ƿ��Ѿ�ȫ���ɽ�
		
		*/
		bool IsOrderTraded(string instrumentid, TThostFtdcDirectionType direction, TThostFtdcOffsetFlagType offset, string orderrefid, int frontid, int sessionid);

		bool IsOrderCanceled(string instrumentid, string orderrefid, int frontid, int sessionid);
	
		double GetOrderAvgPrice(string instrumentid, TThostFtdcDirectionType direction, TThostFtdcOffsetFlagType offset);
		/*
		����instrumentid �Ϸ����ò�λ���
		@instrumentid
		@direction ��շ���
		*/
		CThostFtdcInvestorPositionField GetPostion(string instrumentid,TThostFtdcDirectionType direction);
		/*
		����instrumentid direction ��ò�λ����ϸ���
		@instrumentid ��Լid
		@direction  ��շ���
		*/
		vector<CThostFtdcInvestorPositionDetailField> GetPostionDetail(string instrumentid, TThostFtdcDirectionType direction);


		/*
		���� direction ��ò�λ����ϸ���
	
		@direction  ��շ���
		*/
		vector<CThostFtdcInvestorPositionDetailField> GetPostionDetail( TThostFtdcDirectionType direction);


		/*
		����instrumentid direction ��óֲֵ�ƽ���۸�
		@instrumentid ��Լid
		@direction ����
		*/
		double GetPostionDetailAvgPrice(string instrumentid, TThostFtdcDirectionType direction);

		/*
		����instrumentid direction ��λ����
		@instrumentid ��Լid
		@direction  ��շ���
		*/
		int GetPostionDetailVolume(string instrumentid, TThostFtdcDirectionType direction);

		// ����ɳֲֵ�instrument �б�
		set<string> GetInstrumentIdListHasPostion();


		set<string> GetInstrumentIdListHasPostion(TThostFtdcDirectionType direction);

		int GetPostionDetailVolume(string instrumentid);

		/*
		����instrumentid direction tradingday�ǽ��� ��λ����
		@instrumentid ��Լid
		@direction  ��շ���
		*/
		int GetPostionDetailVolumeToday(string instrumentid, TThostFtdcDirectionType direction,string today);

		/*
		����instrumentid direction tradingday���ǽ��� ��λ����
		@instrumentid ��Լid
		@direction  ��շ���
		*/
		int GetPostionDetailVolumeYesterday(string instrumentid, TThostFtdcDirectionType direction,string today);

		int GetPostionDetailVolume();

	

			/*
		����instrumentid direction ��λ����
		@instrumentid ��Լid
		@direction  ��շ���
		*/
		int GetPostionDetailCount(string instrumentid, TThostFtdcDirectionType direction);


		int GetPostionDetailCount();



	
		/*
		��ùҵ���������� 
		*/
		int GetOrderInputCount();
		/*
		���ݻ�ùҵ����������
		@instrumentid  ��Լid
		@direction ����
		@offset  ��ƽ����
		*/
		int  GetOrderInputCount(string instrumentid, TThostFtdcDirectionType direction, TThostFtdcOffsetFlagType offset);


		int GetOrderInputCount(string instrumentid, TThostFtdcOffsetFlagType offset);
		int GetOrderInputCount(string instrumentid);
		/*��ó������������
		*/
		int GetActionInputCount();
	};

}