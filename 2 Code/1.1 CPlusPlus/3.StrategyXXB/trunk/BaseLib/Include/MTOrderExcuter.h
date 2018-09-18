#pragma once
#include "BaseDataType.h"
#include  "AbstractStrategy.h"
namespace Abstract {

  	class BASELIB_API MTOrderExcuter  {
	public : 
	void 	SetStrategy(AbstractStrategy* strategy);
	/*�����ݿ��н�OrderList ���¼�������*/
	void    LoadMTOrderList();
	//���һ��MTOrder�����ɹ��ͷ���true
	bool   AddMTOrder(MTOrder mtorder);
	//����һ��MTOrder �ɹ��ͷ���true ����mtorderid������
	bool  EditMTOrder(MTOrder mtorder);
    //ɾ��һ��MTOrder �ɹ�����true 
	bool DeleteMTOrder(string mtorderid);

	bool CloseMTorderForce(string mtorderid); //ǿ��ƽ��
	// ���һ��MTOrder ͨ��magicid������е�id
	vector<MTOrder> GetMTOrdersByMagicNum(int magicnum);
	vector<MTOrder> GetALLMTOrder();
	// ����mtorderid ���MTOrder
	MTOrder  GetMTOrderByID(string mtorderid);

	vector<MTOrder> GetMTOrdersByStatus(MTOrderStatus mtorder);

	 void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	 //������ͨ����
	 void DealNormalMTOrder(MTOrder &order, CThostFtdcDepthMarketDataField *pDepthMarketData);
	 //�������ֹ��ĵ���
	 void DealTrailingStopMTOrder(MTOrder&order, CThostFtdcDepthMarketDataField *pDepthMarketData); //

	 //�������ֹӯ�ĵ���
	 void DealTrailingCloseMTOrder(MTOrder&order, CThostFtdcDepthMarketDataField *pDepthMarketData);

	 //�������ֲ�����
	 void SetMaxPostion(string instrumentid, int maxlockvolume);

	 static HxEventEngine e;

  private :	  
	  map<string, int> maxpostionmap;  //��������ֲ��� ��������ģʽ �����������Ĭ�ϲ���������ģʽ
	AbstractStrategy * strategy;
	  vector<MTOrder> vec_mtorder;
	  void CloseMTOrder(MTOrder &mtorder, CThostFtdcDepthMarketDataField *pDepthMarketData, MTOrderCloseReason reason);
	  void CheckCloseMTOrder(MTOrder &mtorder, CThostFtdcDepthMarketDataField *pDepthMarketData);
	  //��һ��mtorder �ĵ���
	  void OpenMTOrder(MTOrder &mtorder, CThostFtdcDepthMarketDataField *pDepthMarketData);
	  void CheckOpenMTOrder(MTOrder &mtorder, CThostFtdcDepthMarketDataField *pDepthMarketData);



	};

}
