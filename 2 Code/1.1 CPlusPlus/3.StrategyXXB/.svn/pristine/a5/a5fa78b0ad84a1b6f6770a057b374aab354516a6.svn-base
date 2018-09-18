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
//k线生成器 
//输入Tick数据生成K线 
namespace Abstract {
	class AbstractBarStrategy;
}
 class BASELIB_API KBarGenerater
{
	 friend class AbstractBarStrategy;
public:
   KBarGenerater(int size=1000);//默认存储1000个个每个周期的bar
   void	OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
   virtual ~KBarGenerater();
   HxEventEngine event;
   typedef std::function<void(TKDATA)> Pf_OnBar;
   Pf_OnBar pf_onbar;
   public:
	   deque<TKDATA>  GetBarList(int period);//得到period 分钟的k线  只返回已经完成的bar
											 /*
											 融合数据  klist 需要融合的数据
											 period 周期
											 updateold 如果时间坐标一样是否替换原有的那个数据

											 */

	      /*
		  length 长度　所谓长度就是最后length 个kdata
		  */
	   deque<TKDATA>  GetBarList(int period,int length);

	   /*周期period 
	     length 长度  数组的长度是length
		 field 字段

	   */
	   shared_ptr<double>  GetBarFieldArray(int period, int length,KBarField field);

	   void  MergerData(deque<TKDATA> klist, int period, bool updateold); //融合数据
	   void  MergerData(shared_ptr<vector<TKDATA>> klist, int period, bool updateold); //融合数据
	  /* void  SetInstrumentMap(shared_ptr<AbstactInstrumentMap>  instrumentMap);*/
	   void Reset();
   private: 
	        deque<TKDATA> list_kdata_m1;//一份钟k线队列
			deque<TKDATA> list_kdata_m5;// 5分钟k线队列
			deque<TKDATA> list_kdata_m15;//15分钟K线队列
			deque<TKDATA> list_kdata_m30;//30分钟k线队列
			deque<TKDATA> list_kdata_h1; //一小时k线队列
			deque<TKDATA> list_kdata_h4; //4小时k线队列
			deque<TKDATA> list_kdata_w1;  //周线
		//	deque<TKDATA> list_kdata_m1;   //月线

			CThostFtdcDepthMarketDataField lasttick;//最新一跳
			/*shared_ptr<AbstactInstrumentMap>  instrumentMap;*/
			TKDATA lastkdata_m1;   //最近一个正在进行的1分钟k线
			TKDATA lastkdata_m5;   //最近一个正在进行的5分钟k线
			TKDATA lastkdata_m15;   //最近一个正在进行的5分钟k线
			TKDATA lastkdata_m30;   //最近一个正在进行的30分钟k线
			TKDATA lastkdata_h1;    //最近一个正在进行的一小时的k线
			TKDATA lastkdata_h4;    //最近一个正在进行的4小时k线
			TKDATA lastkdata_w1;    //最近一个正在进行的周k线
		//	TKDATA lastkdata_m1;    //最近一个正在进行的月k线
			int size;
};

