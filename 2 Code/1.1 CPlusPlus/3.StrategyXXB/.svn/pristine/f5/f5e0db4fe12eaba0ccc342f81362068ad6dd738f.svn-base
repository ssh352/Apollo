#pragma once
#include "BaseDataType.h"
#include  "AbstractStrategy.h"
#include <list>
using namespace std;
using namespace Abstract;
namespace Abstract {
	//用来执行原始的单子保证下单至前置机  支持所有交易所的anyprice 操作只要把limit price 设置成0即可
	class BASELIB_API OrderExcuter {

	public:
	  void 	SetStrategy(AbstractStrategy* strategy); //设置执行的策略
	  void	AddOrder(CThostFtdcInputOrderField orderinput);
	  OrderExcuter();
	  void  OnDepathMarket(CThostFtdcDepthMarketDataField *pDepthMarketData);
	  bool  HasOrder(); //是不是还没有执行的单子
	  void  PrintOrder(string filepath); //将单打印到log
	  void SetIsExcute(bool isexcute); //是否执行下单 true 执行false 不执行

	private:
		AbstractStrategy* strategy;
		list<CThostFtdcInputOrderField> vec_inputorder;
		bool isexcute; //是否需要执行




	};






}