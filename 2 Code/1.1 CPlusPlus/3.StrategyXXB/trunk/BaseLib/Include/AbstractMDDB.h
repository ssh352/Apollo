#pragma once
#include "BaseDataType.h"
#include<memory>
#include<vector>
using namespace std;
namespace Abstract {
	//������������ݿ�ӿ�
	 class BASELIB_API AbstractMDDB { //ע�����abstractdb ֻ��������oninit �����������
	public:		
		//���þ���ʱ��
		virtual	shared_ptr<vector<TKDATA>>  GetKdata(string dbname,string instrumentid, string starttime, string endtime, string period)=0;

	//�������ʱ��
		virtual	shared_ptr<vector<TKDATA>>  GetKdata(string dbname, string instrumentid, int start, int  end, string period)=0;

	  //���þ���ʱ��
		virtual shared_ptr<vector<TKDATA>>  GetTick(string dbname,string instrumentid, string starttime, string endtime)=0;
	  //�������ʱ��
		virtual shared_ptr<vector<TKDATA>>  GetTick(string dbname,string instrumentid, int start, int end)=0;
		//��TradingDayʱ��ȡ
		virtual	shared_ptr<vector<TKDATA>>  GetKdataByTradingday(string dbname, string instrumentid, string starttime, string endtime, string period) = 0;
	};


}