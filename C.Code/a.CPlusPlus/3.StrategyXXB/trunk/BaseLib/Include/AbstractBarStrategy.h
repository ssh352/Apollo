#pragma once
#include "AbstractStrategy.h"
#include "KBarGenerater.h"
#include "KBarGenerater1.h"
#include "MTOrderExcuter.h"
#include "LastTickMap.h"
#include "KBarMap.h"
#include <mutex>
using namespace std;
using namespace Abstract;
namespace Abstract {


	/* �ó������ܲ���onbar �¼�*/
	// todo:��δ���������������ʵ��������
	/* */
	class BASELIB_API AbstractBarStrategy:public AbstractStrategy{
	public:
		AbstractBarStrategy();
		virtual ~AbstractBarStrategy();
		vector<MTOrder> GetAllMTorder();
		bool EditMTOrder(MTOrder mtorder);
		bool DeleteMTOrder(string mtorderid);
		bool CloseMTorderForce(string mtorderid);
		MTOrder GetMTOrderByID(string mtorderid);
	protected:
		virtual void OnBar(TKDATA kbar) = 0;
		virtual void OnRtnDepthMarketDataV(CThostFtdcDepthMarketDataField *pDepthMarketData) = 0;//�ȵ��ô˺������Ż��ٵ���OnBar ����OnRtnDepthMarketData�Ĺ��ܡ���Ϊԭ�ȵĺ�����Ҫ��һ�����涯�� //�Ƿ���Ҫ��������� ������������������ʵ�������ƹҵ��Ĺ�����
		virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData) final;//
		virtual void OnInit() final;
		virtual void OnTime(tm t) final;
		virtual void OnTimeV(tm t);
		virtual void OnInitV();
		virtual void OnReset();
		virtual void CanMerge(string instrumentid,string start,string end) final;//K�߼ӹ���׼���ã����Խ��ܵ�һ��K���ں�
		virtual void FinishMerge() final;
	protected:

		MTOrderExcuter* mtorderexcuter;  //mt
		LastTickMap* lasttickmap;
	protected:
	
		KBarMap * kbarmap;
	private:
	
		bool hasfirstmerge;
	};



}