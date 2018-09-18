#pragma once
#include <string>
#include "BaseLib.h"
using namespace std;
struct BASELIB_API TKDATA {
	int64_t  time;
	double open;
	double close;
	double low;
	double high;
	double amount;
	double volume;
	int periodtype; //周期类型
	char actionday[9]; //业务日期
	char tradingday[9];
	char instrumentid[31];
	int64_t actiondaytimestamp;
	int64_t tradingdaytimestamp;
};
struct BASELIB_API ClosePrice {
	char instrumentid[31];//品种
	double closeprice; //收盘价
	double settlmentprice;//结算价
};
enum BASELIB_API PeriodType {  //时间周期类型    //todo::涉及到周期的函数都要改成PeriodType为参数
	PERIOD_M1 = 1, //一分钟
	PERIOD_M5 = 2,  // 5分钟
	PERIOD_M15 = 3,  // 15分钟
	PERIOD_M30 = 4, // 30分钟
	PERIOD_H1 = 5,  // 1小时
	PERIOD_H4 = 6,  // 4 小时
	PERIOD_D1 = 7,  // 日
	PERIOD_W1 = 8,  //周
	PERIOD_MN1 = 9,  //月
};







enum BASELIB_API KBarField {
	LOW = 1,      //低点
	HIGH = 2,     //高点
	OPEN = 3,     //开盘价
	CLOSE = 4,    //收盘价
	TIMESTAMP = 5 //时间戳
};


struct BASELIB_API InTdTimeTemplet
{
	char instrid[31];    //品种ID
	char instrname[31];  //品种中文名
	char exchid[9];     //交易所ID
	char exchname[61];   //交易所中文名
	int nightend;        //夜盘结束时间
};


enum BASELIB_API MTOrderType {  //MTOrder的类型
	Normal = 1,//普通单子按止盈止损价平单         
	TrailingStop = 2,//追踪止损 在价格达到closeprice 后 激活 但是并不平仓 而是将止损位上移 修改stop price 与单前价格相差winprofit 修改close price = stop price+winprofit;
	TrailingClose = 3,//追踪止盈 在价格达到closepirce 后 激活 但是并不平仓 而是将止盈价格上升 在激活的情况下 如果价格再最高点回落 drawdown 进行止盈
	NONE = 0,//不存在的单子


};

struct BASELIB_API StrategyDBO { //策略概要的存部分

	int id;  //策略的id
	double initfundunit; //初始资金
	double closeprofit;//平仓盈亏
	double floatprofit;    // 浮动盈亏
	double fee;  //费用
	double margin;//保证金
	int fundunit;  //份额
	double cash; //现金
	double netvalue;  //净值
	char  settlementdate[10];     //结算日期  //初始资金+平仓盈亏+浮动盈亏-费用=净值   现金+保证金
};
enum BASELIB_API MTOrderStatus {  //MTOrder的类型
	
	UnOpen = 1,//还没有开仓
	Canceled=0, //在撤销
	Opening = 2,//正在开仓
	Opened = 3,//已开仓
	//
	Active = 4,// 跟踪止盈止损激活
	Closing = 5,//正在平仓
	Closed = 6,   //已经平仓
	CloseForce  //强制平仓

};
enum BASELIB_API MTOrderCancelReason   //MTorder 单子撤销的原因
{
	Force = 0,// 强行撤销
	OverSlipe = 1, //滑点过大
	Expiration = 2  //超时
};

enum BASELIB_API MTOrderDirect {  //MTOrder的类型
	Long = 0,//多
	Short = 1//空
};

enum BASELIB_API MTOrderCloseReason  //平仓原因
{   
	ForceClose=0,//强制平仓
    CloseLoss=1,//止损
	CloseProfit=2,//止盈
};

enum BASELIB_API CloseStatus //平仓状态
{
	unclose=0, //未开始平仓
    closing=1,//开始平仓
	closed=2  //平仓完成
};
enum BASELIB_API OpenStatus  //开仓状态
{
	unopen = 0,
    opening = 1,
	opened=2
};
struct MTOrderMapping{ //mtorder 到ctp order 的映射

	char openorderrefid[13];    //open 单子对应的refid;
	int opensessionid;      //open 单子对应的sessionid
	int openfrontid;      //open 单子对应的frontid
	char opentradingday[9];
	int openvolume;//开仓的数量
	OpenStatus openstatus;
	//char opensessionid        //开仓的session  id
	char closetodayorderrefid[13]; //closetoday 单子对应的refid;  
	int closetodayvolume; //平今的数量
	char closetodaytradingday[9];
	CloseStatus closetodaystatus; //平今的状态
	char closeyestodayorderrefid[13];
	int closeyestodayvolume; //平昨的数量
	CloseStatus closeyestodystatus;//平昨的状态
	char closeyestodaytradingday[9];
	int closetodaysessionid;
	int closetodayfrontid;
	int closeyestodaysessionid;
	int closeyestodayfrontid;
	time_t openinserttime;//ctp 开单插入时间
	time_t closetodayinserttime; //闭合单插入的时间
	time_t closeyestodayinserttime;//平昨插入时间
};


 struct BASELIB_API MTOrder{  //MT4 下单类型
	 char instrumentid[31];//品种id
	 char instrumentname[31];//品种中文名 这个系统只会在界面显示时候使用 不会影响业务逻辑
	 int strategyid;
	 char mtorderid[64];//单据的id
	 int  magicnum; //魔法数字主要是用给策略区分自己的业务逻辑用的
	 time_t inserttime;//单据插入时间
	 MTOrderDirect direct;//多空方向
	 MTOrderType ordertype;  //;下单类型
	 MTOrderMapping mtorderopenmapping;
	 //char opensessionid        //开仓的session  id
	 MTOrderMapping mtorderclosemapping;
	double winprofit;  //在移动止损 移动止盈中用到 止盈价差
	//double lossprofit;
	double openprice;			   //;下单开仓价格
	double realopenprice;          //真实开仓价格。
	double stopprofitprice;      //止盈价位
	double stoplossprice;       //止损价位
	double realcloseprice;     //真实平仓价格
	double drawdown;     //在跟踪止盈的单子里用到 当激活后从止盈处回落drawdown点后 进行止盈
	MTOrderStatus orderstatus; //单子的状态
	int volume;// 手数
	int volumeopened;//已经开仓的手数
	int volumeclosed; //已经平仓的手数
	double  movepage; //移动止损 滑动窗 移动止损激活后当价格超过止盈价movepage 后进行跟踪止损
	double  slippage; // 在开市场单时市场价为的最大允许滑点数
	time_t canceltime;//撤销时间 
	MTOrderCancelReason cancelreason; //撤销原因
	time_t opentime;
	MTOrderCloseReason closereason; //闭合的原因
	
	
	time_t  closetime;//闭合时间  闭合时间超过如果超过一天那么清理掉。
	time_t  expirationtime; //单子作废时间  没有开仓作废？
	char comment[100];//注释
};

