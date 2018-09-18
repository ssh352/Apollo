#pragma once
#include <memory>
#include "BaseLib.h"
#include "AbstractBarStrategy.h"
#include "AbstractChart.h";
/*
 ��ͼ�깦�ܵĳ�����  �����̳���barStrategy ͬʱͨ��chart ���л�ͼ
*/
using namespace Abstract;
namespace Abstract {
	class BASELIB_API AbstractChartStrategy:public AbstractBarStrategy {
	public:
		
		virtual void SetChart(AbstractChart* chart);
		AbstractChartStrategy();
	protected:
		AbstractChart* chart;
	};

}
