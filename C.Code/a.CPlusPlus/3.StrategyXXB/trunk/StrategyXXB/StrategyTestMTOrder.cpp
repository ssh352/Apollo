#include "stdafx.h"
#include "StrategyTestMTOrder.h"
#include "json.hpp"
using nlohmann::json;
StrategyTestMTOrder::StrategyTestMTOrder() 
{  
	this->strategyid = 1011;
	this->strategyname = "����MTOrder";
	this->subscribelist.push_back("p1709");
	json j = { "foo", 1, 2, 3, false,{ { "one", 1 } } };
	
}
StrategyTestMTOrder::~StrategyTestMTOrder()
{

}

void StrategyTestMTOrder::OnRtnTrade(CThostFtdcTradeField *pTrade)
{

}
void StrategyTestMTOrder::OnRspOrderAction(CThostFtdcInputOrderActionField *pInputOrderAction, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	

}
void StrategyTestMTOrder::OnRtnDepthMarketDataV(CThostFtdcDepthMarketDataField *pDepthMarketData)
{

	

}
void  StrategyTestMTOrder::OnRspOrderInsert(CThostFtdcInputOrderField *pInputOrder, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{

}

void StrategyTestMTOrder::OnBar(TKDATA kbar)
{
	//�ҽ������
	//��ע30�������� �������ڵ�bar ȫ��ȥ����
	MTOrder order;
	memset(&order, 0, sizeof(MTOrder));
	memcpy(&order.instrumentid, kbar.instrumentid, sizeof(MTOrder::instrumentid));
	order.direct = MTOrderDirect::Long;
	order.ordertype = MTOrderType::Normal;
	order.orderstatus = MTOrderStatus::UnOpen;
	order.openprice = kbar.close;
	order.stopprofitprice = order.openprice + 6.0;
	order.stoplossprice = order.openprice - 6.0;
	order.volume = 1;
	this->mtorderexcuter->AddMTOrder(order);

	this->logger->Log("������ һ��MTorder open �۸�%f ֹ��%f ֹӯ%f",order.openprice,order.stoplossprice,order.stopprofitprice);
	//this->instrumentmap.getInstrumentid(string(kbar.instrumentid));		
	//order.magicnum = this->GetNewOrderRef();



	


}
void  StrategyTestMTOrder::OnRtnOrder(CThostFtdcOrderField *pOrder)
{

}
void StrategyTestMTOrder::OnTimeV(tm t)
{

	if (t.tm_sec == 0) {
		MTOrder order;
		memset(&order, 0, sizeof(MTOrder));
		memcpy(&order.instrumentid, "p1709", sizeof(MTOrder::instrumentid));
		order.direct = MTOrderDirect::Long;
		order.ordertype = MTOrderType::Normal;
		order.orderstatus = MTOrderStatus::UnOpen;
		order.openprice = this->lasttickmap->getLastTick("p1709").LastPrice;
		order.stopprofitprice = order.openprice + 6.0;
		order.stoplossprice = order.openprice - 6.0;
		order.volume = 1;
		this->mtorderexcuter->AddMTOrder(order);

		this->logger->Log("������ һ��MTorder open �۸�%f ֹ��%f ֹӯ%f", order.openprice, order.stoplossprice, order.stopprofitprice);
	}


}
void StrategyTestMTOrder::OnInitV()
{

}