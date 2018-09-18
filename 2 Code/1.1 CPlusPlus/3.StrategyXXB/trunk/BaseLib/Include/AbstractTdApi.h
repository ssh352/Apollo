#pragma once
#include "ThostFtdcTraderApi.h"
namespace Abstract{
	class BASELIB_API AbstractTdApi
	{
	public:
		AbstractTdApi();
		virtual ~AbstractTdApi();

		///�����ѯ����
		virtual int ReqQryOrder(CThostFtdcQryOrderField *pQryOrder, int nRequestID) = 0;

		virtual int ReqOrderInsert(CThostFtdcInputOrderField *pInputOrder, int nRequestID)=0;

		///Ԥ��¼������
		virtual int ReqParkedOrderInsert(CThostFtdcParkedOrderField *pParkedOrder, int nRequestID)=0;

		///Ԥ�񳷵�¼������
		virtual int ReqParkedOrderAction(CThostFtdcParkedOrderActionField *pParkedOrderAction, int nRequestID)=0;

		///������������
		virtual int ReqOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, int nRequestID)=0;

		///����ɾ��Ԥ��
		virtual int ReqRemoveParkedOrder(CThostFtdcRemoveParkedOrderField *pRemoveParkedOrder, int nRequestID)=0;

		///����ɾ��Ԥ�񳷵�
		virtual int ReqRemoveParkedOrderAction(CThostFtdcRemoveParkedOrderActionField *pRemoveParkedOrderAction, int nRequestID)=0;

		///ִ������¼������
		virtual int ReqExecOrderInsert(CThostFtdcInputExecOrderField *pInputExecOrder, int nRequestID)=0;

		///ִ�������������
		virtual int ReqExecOrderAction(CThostFtdcInputExecOrderActionField *pInputExecOrderAction, int nRequestID)=0;

		///ѯ��¼������
		virtual int ReqForQuoteInsert(CThostFtdcInputForQuoteField *pInputForQuote, int nRequestID)=0;

		///����¼������
		virtual int ReqQuoteInsert(CThostFtdcInputQuoteField *pInputQuote, int nRequestID)=0;

		///���۲�������
		virtual int ReqQuoteAction(CThostFtdcInputQuoteActionField *pInputQuoteAction, int nRequestID)=0;

		///�������¼������
		virtual int ReqCombActionInsert(CThostFtdcInputCombActionField *pInputCombAction, int nRequestID)=0;
	};
}
