#pragma once
#include <ThostFtdcMdApi.h>
#include <ThostFtdcTraderApi.h>
#include <ThostFtdcUserApiStruct.h>
#include <deque>
#include <map>
#include <memory>
#include <time.h>
#include "BaseDataType.h"
#include "AbstractInstrumentMap.h"
#include "HxEventEngine.h"
#include "BaseDataType.h"
using namespace CTPTOOL;
using namespace Abstract;
using namespace std;
//k�������� 
//����Tick��������K�� 
namespace Abstract {
	class AbstractBarStrategy;
}
 class BASELIB_API KBarGenerater
{
	 friend class AbstractBarStrategy;
public:
   KBarGenerater(int size=1000);//Ĭ�ϴ洢1000����ÿ�����ڵ�bar
   void	OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
   virtual ~KBarGenerater();
   HxEventEngine event;
   typedef std::function<void(TKDATA)> Pf_OnBar;
   Pf_OnBar pf_onbar;
   public:
	   deque<TKDATA>  GetBarList(int period);//�õ�period ���ӵ�k��  ֻ�����Ѿ���ɵ�bar
											 /*
											 �ں�����  klist ��Ҫ�ںϵ�����
											 period ����
											 updateold ���ʱ������һ���Ƿ��滻ԭ�е��Ǹ�����

											 */

	      /*
		  length ���ȡ���ν���Ⱦ������length ��kdata
		  */
	   deque<TKDATA>  GetBarList(int period,int length);

	   /*����period 
	     length ����  ����ĳ�����length
		 field �ֶ�

	   */
	   shared_ptr<double>  GetBarFieldArray(int period, int length,KBarField field);

	   void  MergerData(deque<TKDATA> klist, int period, bool updateold); //�ں�����
	   void  MergerData(shared_ptr<vector<TKDATA>> klist, int period, bool updateold); //�ں�����
	  /* void  SetInstrumentMap(shared_ptr<AbstactInstrumentMap>  instrumentMap);*/
	   void Reset();
   private: 
	        deque<TKDATA> list_kdata_m1;//һ����k�߶���
			deque<TKDATA> list_kdata_m5;// 5����k�߶���
			deque<TKDATA> list_kdata_m15;//15����K�߶���
			deque<TKDATA> list_kdata_m30;//30����k�߶���
			deque<TKDATA> list_kdata_h1; //һСʱk�߶���
			deque<TKDATA> list_kdata_h4; //4Сʱk�߶���
			deque<TKDATA> list_kdata_w1;  //����
		//	deque<TKDATA> list_kdata_m1;   //����

			CThostFtdcDepthMarketDataField lasttick;//����һ��
			/*shared_ptr<AbstactInstrumentMap>  instrumentMap;*/
			TKDATA lastkdata_m1;   //���һ�����ڽ��е�1����k��
			TKDATA lastkdata_m5;   //���һ�����ڽ��е�5����k��
			TKDATA lastkdata_m15;   //���һ�����ڽ��е�5����k��
			TKDATA lastkdata_m30;   //���һ�����ڽ��е�30����k��
			TKDATA lastkdata_h1;    //���һ�����ڽ��е�һСʱ��k��
			TKDATA lastkdata_h4;    //���һ�����ڽ��е�4Сʱk��
			TKDATA lastkdata_w1;    //���һ�����ڽ��е���k��
		//	TKDATA lastkdata_m1;    //���һ�����ڽ��е���k��
			int size;
};

