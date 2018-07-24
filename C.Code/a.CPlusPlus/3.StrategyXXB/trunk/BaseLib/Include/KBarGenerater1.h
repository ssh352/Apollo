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
	int serial;//��ʱ���±������������
	int realserial;//��ʵ��ţ���βת����
	int part;//������һ������  ��
};
struct Kstruct
{
	double volume;//�ɽ���
	double openinterest;//
	string tradingday; //������
	int indexK;      //��ǰ��ʱ������
	int lastindexK;//��һ��ʱ������
	int endtime; //��ʼʱ�� �ڼ����� һ������ĵڼ����ӡ�stxh ��ӵ�ע�ʹ��� 
	int starttime; //����ʱ��  �ڼ����� һ������ĵڼ����ӡ�stxh ��ӵ�ע�ʹ��� 
	map<long, Serial> serial;  //ʱ�������������ʵ���.stxh ��ӵ�ע�ʹ��� 
	map<long, int> prepareindex; //׼��ʱ�� �����׼��ʱ���ڿ��� ��ô����Ҫ�ں�����.stxh ��ӵ�ע�ʹ��� 
	map<long, int> endindex;      //ÿһ�ڽ����ķ������꣬�����ڵĽڵ�ӳ��.stxh ��ӵ�ע�ʹ��� 
	map<long, int> startindex;   //ÿһ�ڿ�ʼ�ķ������꣬�����ڵĽڵ�ӳ��.stxh ��ӵ�ע�ʹ��� 
	map<long, int> mapindex;    // ��ͨ������������ڵĽڵ�ӳ��.stxh ��ӵ�ע�ʹ��� 
	vector<long> vecindex;   //ʱ������  ������ʱ�����궼�Ƶ��ö�����.stxh ��ӵ�ע�ʹ��� 
	vector<long> vecwithprend; //ʱ������ ����׼��ʱ��08:59 �� stxh ��ӵ�ע�ʹ��� 
};

//k�������� 
//����Tick��������K�� 
namespace Abstract {
	class AbstractBarStrategy;
}
class BASELIB_API KBarGenerater1
{
	friend class AbstractBarStrategy;
public:
	KBarGenerater1(int size = 1000);//Ĭ�ϴ洢1000����ÿ�����ڵ�bar
	void	OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	virtual ~KBarGenerater1();
	void  OnInit(string instrumentid);
	
	HxEventEngine event;//��Ϣ���� �첽���������Ҫʹ����Ϣ����
	TKDATA logK;
	typedef std::function<void(TKDATA)> Pf_OnBar;
	Pf_OnBar pf_onbar;

	typedef std::function<void(string,string,string)> Pf_CanMerge;
	Pf_CanMerge pf_canmerge;// �����ں����ݵ�ʱ����лص� ��Ϊ���ǵ����ݿ���ֻ����1������ɵ�k�� ���Ա���ȵ�һ���ӽ���ʱ��ſ��Խ������ݵ��ں�

	typedef std::function<void()> Pf_FinshMerge;//���庯��ָ��
	Pf_FinshMerge pf_finishmerge;
public:
	deque<TKDATA>  GetBarList(PeriodType period);//�õ�period ���ӵ�k��  ֻ�����Ѿ���ɵ�bar
										  /*
										  �ں�����  klist ��Ҫ�ںϵ�����
										  period ����
										  updateold ���ʱ������һ���Ƿ��滻ԭ�е��Ǹ�����

										  */

										  /*
										  length ���ȡ���ν���Ⱦ������length ��kdata
										  */
	deque<TKDATA>  GetBarList(PeriodType period, int length);
	//�õ�ƫ��offsetλ��length��K��
	deque<TKDATA>  GetBarList(PeriodType period, int length,int offset);
	//��������ʱ���ȡ����ʱ�䵽���̸��˼���period���ڵ�K��
	int aaaShiftBar(int period, time_t time);
	//��������ʱ���ȡ����ʱ�䵽��ǰ���˼���period���ڵ�K��
	int ShiftBar(int period, time_t time, time_t now);
	//�ں�����
	void  MergerData(deque<TKDATA> klist, int period, bool updateold); 
	 //�ں�����
	void  MergerData(shared_ptr<vector<TKDATA>> klist, int period, bool updateold);
	//��һ���ںϵ�һ��K��;
	void  FirstMergerKDATA(shared_ptr<vector<TKDATA>> klist);
	//һ����K�߽�����
	void FinishKDATA();
	//��������K�ߣ�
	void FinishAllKDATA(bool ismerge);
	void Reset();
    //static void  SetTimeModel(shared_ptr<AbstractTdTimeTemplet> timetemplet);
	void SetIsVirtual(bool b);
	void SetDivIndex(int divindex); //���ð��̺�ҹ�̵ķָ��
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
	//���뵱ǰʱ�����ӦK�����ڷ��ص�ǰ������K�߽���ʱ��
	time_t ReturnCurrentBarTime(time_t currenttime, int period);
	//string ReturnCurrentBarTime(string currenttime,int period);
private:
	 //static shared_ptr<AbstractTdTimeTemplet> timetemplet; //ʱ��ģ��
	deque<TKDATA> list_kdata_m1;//һ����k�߶���
	deque<TKDATA> list_kdata_m5;// 5����k�߶���
	deque<TKDATA> list_kdata_m15;//15����K�߶���
	deque<TKDATA> list_kdata_m30;//30����k�߶���
	deque<TKDATA> list_kdata_h1; //һСʱk�߶���
	deque<TKDATA> list_kdata_h4; //4Сʱk�߶���
	deque<TKDATA> list_kdata_d1;  //����
	deque<TKDATA> list_kdata_w1;  //����
//	deque<TKDATA> list_kdata_m1;   //����
	
	CThostFtdcDepthMarketDataField lasttick;//����һ��
	/*shared_ptr<AbstactInstrumentMap>  instrumentMap;*/
	TKDATA tmpkdata_m1;   //���һ�����ڽ��е�1����k��
	TKDATA tmpkdata_m5;   //���һ�����ڽ��е�5����k��
	TKDATA tmpkdata_m15;   //���һ�����ڽ��е�5����k��
	TKDATA tmpkdata_m30;   //���һ�����ڽ��е�30����k��
	TKDATA tmpkdata_h1;    //���һ�����ڽ��е�һСʱ��k��
	TKDATA tmpkdata_h4;    //���һ�����ڽ��е�4Сʱk��
	TKDATA tmpkdata_d1;    //���һ�����ڽ��е�1��k��
	TKDATA tmpkdata_w1;    //���һ�����ڽ��е���k��
//	TKDATA lastkdata_m1;    //���һ�����ڽ��е���k��
	//TKDATA lastkdata_m1;   //��һ��1����k��
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
	int  divindex; //���̺�ҹ�̵ķָ�� added by stxh 20180403 
	//bool 
	//int iii;
protected:
	//�����ַ�������ʱ�����
	time_t GetTimeStamp(string date, string time);
	//���������µ�KDATA��
	TKDATA InitKDATA(time_t time, double open,int period);	
	//��ʼ������KDATA��
	void InitAllKDATA(time_t time, double open);
	//���������������KDATA
	TKDATA UpdateKDATA(TKDATA kdata, time_t time, double price, double volchange);
	//�������������������KDATA
	void UpdateAllKDATA(time_t time, double price, double volchange);
	//����1����KDATAƴ�Ӹ��߼���KDATA
	TKDATA CollectM1(TKDATA nk, TKDATA m1k);
	//����K�ߵ�����
	void PushKDATA(TKDATA kdata);
	//��ʱ���ת����ģ���ڵ�index
	int TimeStampToIndex(time_t time);
	//��ʱ���ת��Ϊ�ַ�ʱ��
	string TimeStampToDateTime(time_t time);
	//��indexת��Ϊʱ���
	time_t IndexToTimeStamp(int index,time_t time_day);
};

