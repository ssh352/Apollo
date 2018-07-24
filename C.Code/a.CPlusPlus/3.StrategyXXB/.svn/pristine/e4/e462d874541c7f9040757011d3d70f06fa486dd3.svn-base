#pragma once
#include <memory>
#include "BaseLib.h"
#include "AbstractBarStrategy.h"
#include "AbstractChart.h";
/*
 带图标功能的抽象类  这个类继承了barStrategy 同时通过chart 进行绘图
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
