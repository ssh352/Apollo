#pragma once
#include <map>
#include <memory>
//�����������Ʒ�ֵ�kbar 
#include "KBarGenerater1.h"
#include "AbstractInstrumentMap.h"
using namespace std;
using namespace Abstract;
namespace Simulate {
	class SimulateServer;

};
 class BASELIB_API KBarMap {
	 friend class AbstractBarStrategy;
	 friend class Simulate::SimulateServer;
private:
	shared_ptr<map<string, shared_ptr<KBarGenerater1>>> kbargeneratermap;//string Ʒ�ֱ�ţ�kbar kbar	
	void OnBar(TKDATA k);
	bool isVirtual;
	virtual void CanMerge(string instrumentid, string start, string end) final;
	virtual void FinishMerge() final;

	map<string, Kstruct> kgenerater;
	shared_ptr<vector<InTdTimeTemplet>> vec_timetemplet;
	static shared_ptr<AbstractTdTimeTemplet> timetemplet; //ʱ��ģ��

public:
	KBarMap();
	~KBarMap();
	shared_ptr<KBarGenerater1>  operator[](string insturmentid);
	shared_ptr<KBarGenerater1>  GetKbar(string instrumentid);

	typedef std::function<void(TKDATA)> Pf_OnBar;
	Pf_OnBar pf_onbar;


	typedef std::function<void(string, string, string)> Pf_CanMerge;
	Pf_CanMerge pf_canmerge;

	typedef std::function<void()> Pf_FinishMerge;
	Pf_FinishMerge pf_finishmerge;

	//K�߼ӹ���׼���ã����Խ��ܵ�һ��K���ں�
	void    OnInit(vector<string> subscribelist);
	void    SetIsVirtual(bool b);
	void	OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData);
	void    OnTime(tm t);
	static void  SetTimeModel(shared_ptr<AbstractTdTimeTemplet> timetemplet);
	void    LoadKgenerater();
	void    AddKBarGenerater(string instrumentid);
	void    Reset();
	/*void  SetInstrumentMap(shared_ptr<AbstactInstrumentMap>  instrumentMap);*/
};