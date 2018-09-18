#pragma once
#include "BaseDataType.h"
#include  "AbstractStrategy.h"
#include <list>
using namespace std;
using namespace Abstract;
namespace Abstract {
	//����ִ��ԭʼ�ĵ��ӱ�֤�µ���ǰ�û�  ֧�����н�������anyprice ����ֻҪ��limit price ���ó�0����
	class BASELIB_API OrderExcuter {

	public:
	  void 	SetStrategy(AbstractStrategy* strategy); //����ִ�еĲ���
	  void	AddOrder(CThostFtdcInputOrderField orderinput);
	  OrderExcuter();
	  void  OnDepathMarket(CThostFtdcDepthMarketDataField *pDepthMarketData);
	  bool  HasOrder(); //�ǲ��ǻ�û��ִ�еĵ���
	  void  PrintOrder(string filepath); //������ӡ��log
	  void SetIsExcute(bool isexcute); //�Ƿ�ִ���µ� true ִ��false ��ִ��

	private:
		AbstractStrategy* strategy;
		list<CThostFtdcInputOrderField> vec_inputorder;
		bool isexcute; //�Ƿ���Ҫִ��




	};






}