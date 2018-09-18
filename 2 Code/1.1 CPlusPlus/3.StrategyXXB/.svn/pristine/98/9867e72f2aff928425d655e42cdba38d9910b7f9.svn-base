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
#include "AbstractTdTimeTemplet.h"
#include "LOGGER.h"
//#include "CtpRuntime.h"

using namespace CTPTOOL;
using namespace Abstract;
using namespace std;



struct Serial
{
	int serial;//用时间下标计算出来的序号
	int realserial;//真实序号（首尾转换后）
	int part;//属于哪一段行情  节
};
struct Kstruct
{
	double volume;//成交量
	double openinterest;//
	string tradingday; //交易日
	int indexK;      //当前的时间坐标
	int lastindexK;//上一个时间坐标
	int endtime; //开始时间 第几分钟 一天里面的第几分钟。stxh 添加的注释存疑 
	int starttime; //结束时间  第几分钟 一天里面的第几分钟。stxh 添加的注释存疑 
	map<long, Serial> serial;  //时间坐标与序号真实序号.stxh 添加的注释存疑 
	map<long, int> prepareindex; //准备时间 如果在准备时间内开启 那么不需要融合数据.stxh 添加的注释存疑 
	map<long, int> endindex;      //每一节结束的分钟坐标，和属于的节的映射.stxh 添加的注释存疑 
	map<long, int> startindex;   //每一节开始的分钟坐标，和属于的节的映射.stxh 添加的注释存疑 
	map<long, int> mapindex;    // 普通分钟坐标和属于的节的映射.stxh 添加的注释存疑 
	vector<long> vecindex;   //时间坐标  将所有时间坐标都推到该队列里.stxh 添加的注释存疑 
	vector<long> vecwithprend; //时间坐标 包括准备时间08:59 ？ stxh 添加的注释存疑 
};

//k线生成器 
//输入Tick数据生成K线 
namespace Abstract {
	class AbstractBarStrategy;
}
class BASELIB_API KBarGenerater1
{
	friend class AbstractBarStrategy;
public:
	KBarGenerater1(int size = 1000);//默认存储1000个个每个周期的bar
	void	OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	virtual ~KBarGenerater1();
	void  OnInit(string instrumentid);
	
	HxEventEngine event;//消息引擎 异步的情况下需要使用消息引擎
	TKDATA logK;
	typedef std::function<void(TKDATA)> Pf_OnBar;
	Pf_OnBar pf_onbar;

	typedef std::function<void(string,string,string)> Pf_CanMerge;
	Pf_CanMerge pf_canmerge;// 可以融合数据的时候进行回调 因为我们的数据库里只保存1分钟完成的k线 所以必须等到一分钟结束时候才可以进行数据的融合

	typedef std::function<void()> Pf_FinshMerge;//定义函数指针
	Pf_FinshMerge pf_finishmerge;
public:
	deque<TKDATA>  GetBarList(PeriodType period);//得到period 分钟的k线  只返回已经完成的bar
										  /*
										  融合数据  klist 需要融合的数据
										  period 周期
										  updateold 如果时间坐标一样是否替换原有的那个数据

										  */

										  /*
										  length 长度　所谓长度就是最后length 个kdata
										  */
	deque<TKDATA>  GetBarList(PeriodType period, int length);
	//得到偏移offset位后length个K线
	deque<TKDATA>  GetBarList(PeriodType period, int length,int offset);
	//根据输入时间获取输入时间到开盘隔了几根period周期的K线
	int aaaShiftBar(int period, time_t time);
	//根据输入时间获取输入时间到当前隔了几根period周期的K线
	int ShiftBar(int period, time_t time, time_t now);
	//融合数据
	void  MergerData(deque<TKDATA> klist, int period, bool updateold); 
	 //融合数据
	void  MergerData(shared_ptr<vector<TKDATA>> klist, int period, bool updateold);
	//第一次融合第一根K线;
	void  FirstMergerKDATA(shared_ptr<vector<TKDATA>> klist);
	//一分钟K线结束；
	void FinishKDATA();
	//结束所有K线；
	void FinishAllKDATA(bool ismerge);
	void Reset();
    //static void  SetTimeModel(shared_ptr<AbstractTdTimeTemplet> timetemplet);
	void SetIsVirtual(bool b);
	void SetDivIndex(int divindex); //设置白盘和夜盘的分割点
	void SetKgenerater(Kstruct kgenerater);
	bool HasFinished();
	int GetEndTime();
	void SetFinished(bool b);
	void SetIndexK(int t);
	void SetlastIndexK(int t);
	void SetActionDay(string actionday);
	bool JudgeIsNormal(time_t t);
	bool JudgeIsEnd(time_t t);
	int NextIndex(int index);
	int NextSerial(int index);
	int PreIndex(int index);
	//输入当前时间和相应K线周期返回当前该周期K线结束时间
	time_t ReturnCurrentBarTime(time_t currenttime, int period);
	//string ReturnCurrentBarTime(string currenttime,int period);
private:
	 //static shared_ptr<AbstractTdTimeTemplet> timetemplet; //时间模板
	deque<TKDATA> list_kdata_m1;//一份钟k线队列
	deque<TKDATA> list_kdata_m5;// 5分钟k线队列
	deque<TKDATA> list_kdata_m15;//15分钟K线队列
	deque<TKDATA> list_kdata_m30;//30分钟k线队列
	deque<TKDATA> list_kdata_h1; //一小时k线队列
	deque<TKDATA> list_kdata_h4; //4小时k线队列
	deque<TKDATA> list_kdata_d1;  //日线
	deque<TKDATA> list_kdata_w1;  //周线
//	deque<TKDATA> list_kdata_m1;   //月线
	
	CThostFtdcDepthMarketDataField lasttick;//最新一跳
	/*shared_ptr<AbstactInstrumentMap>  instrumentMap;*/
	TKDATA tmpkdata_m1;   //最近一个正在进行的1分钟k线
	TKDATA tmpkdata_m5;   //最近一个正在进行的5分钟k线
	TKDATA tmpkdata_m15;   //最近一个正在进行的5分钟k线
	TKDATA tmpkdata_m30;   //最近一个正在进行的30分钟k线
	TKDATA tmpkdata_h1;    //最近一个正在进行的一小时的k线
	TKDATA tmpkdata_h4;    //最近一个正在进行的4小时k线
	TKDATA tmpkdata_d1;    //最近一个正在进行的1日k线
	TKDATA tmpkdata_w1;    //最近一个正在进行的周k线
//	TKDATA lastkdata_m1;    //最近一个正在进行的月k线
	//TKDATA lastkdata_m1;   //上一个1分钟k线
	int size;
	int endtime;
	time_t nowtime;
	time_t finishtime;
	time_t newtime;
	int currentindex;
	int dotime;
	string realactionday;
	string subinid;
	string subinidwithoutnum;
	string tmpinstrumentid;
	string updatetime;
	string lasttime_string;
	string actionday;
	string tradingday;
	double tmpaskprice1;
	double tmpbidprice1;
	double tmpaskvolume1;
	double tmpbidvolume1;
	double tmplastprice;
	double finishprice;
	double tmpaverageprice;
	int64_t tmpvolume;
	int64_t tmpvolumechange;
	int64_t tmpopeninterest;
	int64_t tmpopeninterestchange;
	int64_t tmptime;
	//shared_ptr<vector<InTdTimeTemplet>> vec_timetemplet;
	//map<string, Kstruct> kgenerater;
	struct Kstruct tmpkgenerater;
	time_t time_t_tradingday;
	time_t time_t_actionday;
	time_t real_time_t_actionday;
	time_t last_time_t_actionday;
	time_t indextime;
	time_t lasttimestamp;
	time_t timestamp;
	shared_ptr<LOGGER> logger;
	bool canPushKDATA;
	bool hasMergeFirstKDATA;
	double tmpfinishprice;
	bool isPrepare;
	bool isStart;
	bool isEnd;
	bool isNormal;	
	bool isVirtual;
	bool hasFinished;
	bool forceUpdate;
	bool hasModel;
	bool lastIsEnd;
	int  divindex; //白盘和夜盘的分割点 added by stxh 20180403 
	//bool 
	//int iii;
protected:
	//根据字符串返回时间戳；
	time_t GetTimeStamp(string date, string time);
	//重新生成新的KDATA；
	TKDATA InitKDATA(time_t time, double open,int period);	
	//初始化所有KDATA；
	void InitAllKDATA(time_t time, double open);
	//根据行情持续更新KDATA
	TKDATA UpdateKDATA(TKDATA kdata, time_t time, double price, double volchange);
	//根据行情持续更新所有KDATA
	void UpdateAllKDATA(time_t time, double price, double volchange);
	//根据1分钟KDATA拼接更高级的KDATA
	TKDATA CollectM1(TKDATA nk, TKDATA m1k);
	//推送K线到策略
	void PushKDATA(TKDATA kdata);
	//把时间戳转换成模版内的index
	int TimeStampToIndex(time_t time);
	//把时间戳转换为字符时间
	string TimeStampToDateTime(time_t time);
	//把index转换为时间戳
	time_t IndexToTimeStamp(int index,time_t time_day);
};

