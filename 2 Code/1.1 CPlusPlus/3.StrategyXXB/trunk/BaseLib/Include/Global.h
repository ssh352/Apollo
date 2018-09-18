#pragma once
#include "AbstractTimer.h"
#include <thread>
using namespace Abstract;
class BASELIB_API Global
{
private:
	static shared_ptr<AbstractTimer> timer ;    //全局变量
public:
	static void SetTimer(shared_ptr<AbstractTimer> timer);
public:
	static shared_ptr<AbstractTimer> GetTimer();
};