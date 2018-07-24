#pragma once
#include <deque>
#include <thread>
#include<vector>
#include "afxmt.h"
using namespace std;
namespace CTPTOOL {
	enum BASELIB_API eventtype {
		login = 0,
		logappend = 1,
		pricechange = 2,
		accountchange = 3,
		tradelistchange = 4,
		ordelistchange = 5,
		postionchange = 6,
		tick = 7,
		draw = 8,
		bar = 9,//bar　产生事件
		strategystoped = 10,//策略停止事件
		mtorderchange = 11,//mtorder 进行了变更
		strategystart = 12,  //策略启动事件
			
	};

	 enum BASELIB_API HxDealerStatus {
		unstart,
		start,
		starting,
		running,
		stop,
		stoping,
		stoped

	};

	 struct BASELIB_API eventstruct {
		eventtype type;
		string message;
		bool dropable;
		byte data[1000];
	};
	 class BASELIB_API HxDealer {

	public :
		virtual ~HxDealer();
		HxDealer();
	protected :
		virtual void OnHxEvent(eventstruct e) = 0;
		friend class HxEventEngine;
		

	private:
		HxDealerStatus status;
		
		void putEvent(eventstruct e);
		thread* t;
		CRITICAL_SECTION cm;

	
		void eventloop();
		void start();
		
		deque<eventstruct> eventlist;

	};
	 class BASELIB_API HxEventEngine
	{
		//全局事件引擎
	public:
		HxEventEngine();		
		void virtual Adddealer(HxDealer * dealer) final;
		void virtual Movedealer(HxDealer * dealer) final;
		void virtual MoveAlldealer() final;
		void virtual operator()(eventstruct e) ;
		virtual ~HxEventEngine();
		static bool stop;
	private :
		CRITICAL_SECTION cm;
		vector<HxDealer *> dealerlist;
	};

}