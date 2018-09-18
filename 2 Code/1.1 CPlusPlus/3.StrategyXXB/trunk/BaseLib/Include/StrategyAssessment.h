#pragma once
#include <vector>
#include <memory>
#include "ThostFtdcTraderApi.h"
#include "AbstractInstrumentMap.h"
#include "AbstractInstrumentCommissionRateMap.h"
using namespace std;
using namespace Abstract;
namespace Abstract {
	/* ���������� 
	   �ṩ���Ե����۷���
	*/
	class BASELIB_API StrategyAssessment {
	private:
			vector<CThostFtdcTradeField> list_trade;
			shared_ptr<AbstactInstrumentMap> instrumentmap;
			shared_ptr<AbstractInstrumentCommissionRateMap> instrumetncommissionmap;
			double InitalMoney;
			int TradeDays;

	public :
		static vector<pair<string, string>> GetIndicatorNameList();//�õ�Ӣ����-������ָ���б�
	public:
		void SetTradeList(vector<CThostFtdcTradeField> list_trade);
		void SetInstrumentMap(shared_ptr<AbstactInstrumentMap> instrumentmap);
		void SetInstrumentCommissionRateMap(shared_ptr<AbstractInstrumentCommissionRateMap> instrumentcommissionmap);
		void SetBase();//���û�׼
		vector<double> GetNetWorthList();
		map<string,string> GetIndicatorValueMap();//�����Ӣ����-valueֵ
		//map<string,>

	public:
		/*������*/
		double GetCommissionRate();
		///��������ָ��
		/*��ӯ��*/
		double GetTotalProfit();
		/*�ܿ���*/
		double GetTotalLoss();
		/*������*/
		double GetNetProfit();
		/*	ӯ����		*/
		double GetWLRatio();
		///���״�����ָ��
		/*�ܽ��״���*/
		int GetTotalTradeTime();
		/*�ղֵĴ���*/
		int GetEmptyTime();
		/*ӯ������*/
		int GetWinTime();
		/*�������*/
		int GetLossTime();
		/*��ƽ����*/
		int GetEvenTime();
		/*ʤ��*/
		double GetWinPer();
		/*����*/
		double GetLossPer();
		/*�����ӯ����*/
		double GetMaxConWinTime();
		/*�����������*/
		double GetMaxConLossTime();
		///ƽ��������ָ��
		//ƽ��������
		double GetAvrNetProfit();
		//ƽ��ӯ��
		double GetAvrProfit();
		//ƽ������
		double GetAvrLoss();
		//ƽ��ӯ����
		double GetAvrWLRation();
		//ƽ���ֲ�����  ����ʱ����
		int GetAvrHoldingCycle();
		//ƽ���ղ����� ����ʱ����
		int GetAvrEmptyCycle();
		//ӯ������
		int GetWinDays();
		//��������
		int GetLossDays();
		//ƽ��ӯ������ ����ʱ��Ϊ��
		int GetAvrWinCycle();
		//ƽ����������
		int GetAvrLossCycle();
		//ƽ����ƽ����
		int GetEventCycle();
		///���˽�����ָ��

		//��󵥴�ӯ��
		double GetMaxOnceProfit();
		//��󵥴ο���
		double GetMaxOnceLoss();
		//�������ӯ��
		double GetMaxConProfit();
		//�����������
		double GetMaxConLoss();
		//���Ӯ
		double GetMaxOpenProfit();
		//��󸡿�
		double GetMaxOpenLoss();


		//�ʲ��ز�ָ����

		//�س�����
		double GetDrawdownTime();
		//ƽ���س�
		double GetAvrDrawdown();
		//���س�
		double GetMaxDrawdown();
		//���س���
		double GetMaxDrawdownRatio();
		//���س�����
		string GetMaxDrwadownDate();
		//�س���ȫϵ��
		double GetDrawdownSafety();


		///��������ָ��
		//ʵ��������
		double GetActualYield();
		//ƽ��ÿ��������
		double GetAvrDayYield();
		//�껯������
		double GetYearYield();

		///������ָ��
		//������
		double GetVolatility();
		//����ֵ
		double GetBeta();
		//�������
		double GetTrackingError();
		//���ռ�ֵ
		double GetVar();

		///����/������ָ��
		//������
		double GetSharpeRatio();
		//�����ɱ���
		double GetTR();
		//ղɭ������
		double GetAlpha();
		//��Ϣ����
		double GetIR();
		//Sortino Ratio
		double GetSortinoRatio();
		//Carmar�ʣ��껯������ / ���س���
		double GetCalmarRatio();









      };


}