#pragma once
#include<map>
#include<vector>
#include<memory>
#include "ThostFtdcUserApiStruct.h"
#include "BaseDataType.h"
using namespace std;

namespace Abstract {
	class BASELIB_API AbstractTdTimeTemplet
	{
	public:
		AbstractTdTimeTemplet();
		shared_ptr<vector<InTdTimeTemplet>> virtual  LoadTdTimeTempletfromLocalDB() = 0;//�����ݿ�װ�ú�Լ����ʱ��ģ��

		virtual ~AbstractTdTimeTemplet();
	protected:
		vector<InTdTimeTemplet> tdtimetemplet;
		bool hasinit;
	};
}


