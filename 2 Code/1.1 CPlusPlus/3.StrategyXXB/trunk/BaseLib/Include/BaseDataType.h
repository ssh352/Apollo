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
	int periodtype; //��������
	char actionday[9]; //ҵ������
	char tradingday[9];
	char instrumentid[31];
	int64_t actiondaytimestamp;
	int64_t tradingdaytimestamp;
};
struct BASELIB_API ClosePrice {
	char instrumentid[31];//Ʒ��
	double closeprice; //���̼�
	double settlmentprice;//�����
};
enum BASELIB_API PeriodType {  //ʱ����������    //todo::�漰�����ڵĺ�����Ҫ�ĳ�PeriodTypeΪ����
	PERIOD_M1 = 1, //һ����
	PERIOD_M5 = 2,  // 5����
	PERIOD_M15 = 3,  // 15����
	PERIOD_M30 = 4, // 30����
	PERIOD_H1 = 5,  // 1Сʱ
	PERIOD_H4 = 6,  // 4 Сʱ
	PERIOD_D1 = 7,  // ��
	PERIOD_W1 = 8,  //��
	PERIOD_MN1 = 9,  //��
};







enum BASELIB_API KBarField {
	LOW = 1,      //�͵�
	HIGH = 2,     //�ߵ�
	OPEN = 3,     //���̼�
	CLOSE = 4,    //���̼�
	TIMESTAMP = 5 //ʱ���
};


struct BASELIB_API InTdTimeTemplet
{
	char instrid[31];    //Ʒ��ID
	char instrname[31];  //Ʒ��������
	char exchid[9];     //������ID
	char exchname[61];   //������������
	int nightend;        //ҹ�̽���ʱ��
};


enum BASELIB_API MTOrderType {  //MTOrder������
	Normal = 1,//��ͨ���Ӱ�ֹӯֹ���ƽ��         
	TrailingStop = 2,//׷��ֹ�� �ڼ۸�ﵽcloseprice �� ���� ���ǲ���ƽ�� ���ǽ�ֹ��λ���� �޸�stop price �뵥ǰ�۸����winprofit �޸�close price = stop price+winprofit;
	TrailingClose = 3,//׷��ֹӯ �ڼ۸�ﵽclosepirce �� ���� ���ǲ���ƽ�� ���ǽ�ֹӯ�۸����� �ڼ��������� ����۸�����ߵ���� drawdown ����ֹӯ
	NONE = 0,//�����ڵĵ���


};

struct BASELIB_API StrategyDBO { //���Ը�Ҫ�Ĵ沿��

	int id;  //���Ե�id
	double initfundunit; //��ʼ�ʽ�
	double closeprofit;//ƽ��ӯ��
	double floatprofit;    // ����ӯ��
	double fee;  //����
	double margin;//��֤��
	int fundunit;  //�ݶ�
	double cash; //�ֽ�
	double netvalue;  //��ֵ
	char  settlementdate[10];     //��������  //��ʼ�ʽ�+ƽ��ӯ��+����ӯ��-����=��ֵ   �ֽ�+��֤��
};
enum BASELIB_API MTOrderStatus {  //MTOrder������
	
	UnOpen = 1,//��û�п���
	Canceled=0, //�ڳ���
	Opening = 2,//���ڿ���
	Opened = 3,//�ѿ���
	//
	Active = 4,// ����ֹӯֹ�𼤻�
	Closing = 5,//����ƽ��
	Closed = 6,   //�Ѿ�ƽ��
	CloseForce  //ǿ��ƽ��

};
enum BASELIB_API MTOrderCancelReason   //MTorder ���ӳ�����ԭ��
{
	Force = 0,// ǿ�г���
	OverSlipe = 1, //�������
	Expiration = 2  //��ʱ
};

enum BASELIB_API MTOrderDirect {  //MTOrder������
	Long = 0,//��
	Short = 1//��
};

enum BASELIB_API MTOrderCloseReason  //ƽ��ԭ��
{   
	ForceClose=0,//ǿ��ƽ��
    CloseLoss=1,//ֹ��
	CloseProfit=2,//ֹӯ
};

enum BASELIB_API CloseStatus //ƽ��״̬
{
	unclose=0, //δ��ʼƽ��
    closing=1,//��ʼƽ��
	closed=2  //ƽ�����
};
enum BASELIB_API OpenStatus  //����״̬
{
	unopen = 0,
    opening = 1,
	opened=2
};
struct MTOrderMapping{ //mtorder ��ctp order ��ӳ��

	char openorderrefid[13];    //open ���Ӷ�Ӧ��refid;
	int opensessionid;      //open ���Ӷ�Ӧ��sessionid
	int openfrontid;      //open ���Ӷ�Ӧ��frontid
	char opentradingday[9];
	int openvolume;//���ֵ�����
	OpenStatus openstatus;
	//char opensessionid        //���ֵ�session  id
	char closetodayorderrefid[13]; //closetoday ���Ӷ�Ӧ��refid;  
	int closetodayvolume; //ƽ�������
	char closetodaytradingday[9];
	CloseStatus closetodaystatus; //ƽ���״̬
	char closeyestodayorderrefid[13];
	int closeyestodayvolume; //ƽ�������
	CloseStatus closeyestodystatus;//ƽ���״̬
	char closeyestodaytradingday[9];
	int closetodaysessionid;
	int closetodayfrontid;
	int closeyestodaysessionid;
	int closeyestodayfrontid;
	time_t openinserttime;//ctp ��������ʱ��
	time_t closetodayinserttime; //�պϵ������ʱ��
	time_t closeyestodayinserttime;//ƽ�����ʱ��
};


 struct BASELIB_API MTOrder{  //MT4 �µ�����
	 char instrumentid[31];//Ʒ��id
	 char instrumentname[31];//Ʒ�������� ���ϵͳֻ���ڽ�����ʾʱ��ʹ�� ����Ӱ��ҵ���߼�
	 int strategyid;
	 char mtorderid[64];//���ݵ�id
	 int  magicnum; //ħ��������Ҫ���ø����������Լ���ҵ���߼��õ�
	 time_t inserttime;//���ݲ���ʱ��
	 MTOrderDirect direct;//��շ���
	 MTOrderType ordertype;  //;�µ�����
	 MTOrderMapping mtorderopenmapping;
	 //char opensessionid        //���ֵ�session  id
	 MTOrderMapping mtorderclosemapping;
	double winprofit;  //���ƶ�ֹ�� �ƶ�ֹӯ���õ� ֹӯ�۲�
	//double lossprofit;
	double openprice;			   //;�µ����ּ۸�
	double realopenprice;          //��ʵ���ּ۸�
	double stopprofitprice;      //ֹӯ��λ
	double stoplossprice;       //ֹ���λ
	double realcloseprice;     //��ʵƽ�ּ۸�
	double drawdown;     //�ڸ���ֹӯ�ĵ������õ� ��������ֹӯ������drawdown��� ����ֹӯ
	MTOrderStatus orderstatus; //���ӵ�״̬
	int volume;// ����
	int volumeopened;//�Ѿ����ֵ�����
	int volumeclosed; //�Ѿ�ƽ�ֵ�����
	double  movepage; //�ƶ�ֹ�� ������ �ƶ�ֹ�𼤻�󵱼۸񳬹�ֹӯ��movepage ����и���ֹ��
	double  slippage; // �ڿ��г���ʱ�г���Ϊ�������������
	time_t canceltime;//����ʱ�� 
	MTOrderCancelReason cancelreason; //����ԭ��
	time_t opentime;
	MTOrderCloseReason closereason; //�պϵ�ԭ��
	
	
	time_t  closetime;//�պ�ʱ��  �պ�ʱ�䳬���������һ����ô�������
	time_t  expirationtime; //��������ʱ��  û�п������ϣ�
	char comment[100];//ע��
};

