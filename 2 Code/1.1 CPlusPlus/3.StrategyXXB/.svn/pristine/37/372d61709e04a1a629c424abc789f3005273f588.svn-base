#pragma once
#include "ThostFtdcTraderApi.h"
namespace Abstract{
	class BASELIB_API AbstractTdApi
	{
	public:
		AbstractTdApi();
		virtual ~AbstractTdApi();

		///请求查询报单
		virtual int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) = 0;

		virtual int ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID)=0;

		///预埋单录入请求
		virtual int ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID)=0;

		///预埋撤单录入请求
		virtual int ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID)=0;

		///报单操作请求
		virtual int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID)=0;

		///请求删除预埋单
		virtual int ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID)=0;

		///请求删除预埋撤单
		virtual int ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID)=0;

		///执行宣告录入请求
		virtual int ReqExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, int nRequestID)=0;

		///执行宣告操作请求
		virtual int ReqExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, int nRequestID)=0;

		///询价录入请求
		virtual int ReqForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, int nRequestID)=0;

		///报价录入请求
		virtual int ReqQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, int nRequestID)=0;

		///报价操作请求
		virtual int ReqQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, int nRequestID)=0;

		///申请组合录入请求
		virtual int ReqCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, int nRequestID)=0;
	};
}
