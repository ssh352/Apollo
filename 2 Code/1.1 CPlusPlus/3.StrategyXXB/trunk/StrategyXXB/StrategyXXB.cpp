// StrategyDemo.cpp : ���� DLL Ӧ�ó���ĵ���������
//
#include "stdafx.h"
//#define StrategyTestMTOrderON
//#define StrategyFICCON
#define StrategyFICCNON
//#define StrategyFICCWON
#ifdef StrategyFICCNON
#include "StrategyFICCN.h"
#endif

#ifdef StrategyBreakAlertON 
#include "StrategyBreakAlert.h"
#endif

#ifdef StrategyOnBarON
#include "StrategyOnBar.h"
#endif 

#ifdef StrategyFlirtON
#include "StrategyFlirt.h"
#endif
#ifdef StrategyDeviateON
#include "StrategyDeviate.h"
#endif
#ifdef StrategyMA40AlertON
#include "StrategyMA40Alert.h"
#endif
#ifdef StrategyMAON
#include "StrategyMA.h"
#endif
#ifdef StrategyTestMTOrderON
#include "StrategyTestMTOrder.h"
#endif
#ifdef StrategyArbitrageNoticeON
#include "StrategyArbitrageNotice.h"
#endif
#ifdef StrategyGridON
#include "StrategyGrid.h"
#endif
#ifdef StrategyFICCON
#include "StrategyFICC.h"
#endif
#ifdef StrategyFICCNON
#include "StrategyFICCN.h"
#endif
#ifdef StrategyFICCWON
#include "StrategyFICCW.h"
#endif
#ifdef Stragety
#include "StrategyTestMTOrder.h"
#endif
#ifndef STRATEGYDEMO_EXPORTS
#define STRATEGYDEMO_EXPORTS
#endif


#include "StrategyXXB.h"


extern"C" STRATEGYDEMO_API AbstractStrategy* GetStrategyInstance() {

#ifdef StrategyBreakAlertON
	StrategyBreakAlert *p = new StrategyBreakAlert();
#endif;
#ifdef StrategyFlirtON
	StrategyFlirt *p = new StrategyFlirt();
#endif;

#ifdef StrategyDeviateON
	StrategyDeviate *p = new StrategyDeviate();
#endif


#ifdef StrategyOnBarON
	StrategyOnBar *p = new StrategyOnBar();
#endif 

#ifdef StrategyTestMTOrderON
	StrategyTestMTOrder *p = new  StrategyTestMTOrder();
#endif


#ifdef StrategyArbitrageNoticeON
	StrategyArbitrageNotice *p = new  StrategyArbitrageNotice();
#endif

#ifdef StrategyGridON
	StrategyGrid  * p = new StrategyGrid();
#endif
#ifdef  StrategyFICCON
	StrategyFICC  * p = new StrategyFICC();
#endif
#ifdef StrategyFICCWON
	StrategyFICCW  * p = new StrategyFICCW();
#endif

#ifdef StrategyFICCNON
	StrategyFICCN  * p = new StrategyFICCN();
#endif
	return (AbstractStrategy*)p;


}