#pragma once
#include "BaseLib.h"
using namespace std;

namespace Abstract {
	// 抽象chart 类 todo: 奚博士实现



	enum PointShapeType {
		BuyOpen=1,
		BuyClose=2,
		SellOpen=3,
		SellClose=4
	};

	struct Point{
		time_t time;
		double y;
		PointShapeType pointshape;
	};

	class BASELIB_API AbstractChart {
		/* @time_t 时间  以时间为坐标
		   @value 值
		   @color 色彩
		    */
	public:
		virtual void DrawPoint(Point p) = 0;
		virtual void DrawLine()=0;//绘制线断 绘制色彩
		virtual void DrawTextS() = 0;//绘制文本
		virtual void DrawCurve()=0;//绘制曲线
		virtual void DrawArrow() = 0;//绘制箭头
		virtual void DrawCycle() = 0;//绘制圆形
		virtual void DrawTrend() = 0;//绘制射线
	};


}