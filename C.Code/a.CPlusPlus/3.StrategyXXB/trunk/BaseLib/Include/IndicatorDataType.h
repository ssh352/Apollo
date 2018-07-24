#pragma once
#include <string>
using namespace std;
 namespace Indicator {
	  class BASELIB_API CBars
		{
		public:
			CBars();
			~CBars();
			void initial();
		public:
			string strDate;
			string strTime;
			int iPeriods;

			double dOpenPrice;
			double dClosePrice;
			double dHighPrice;
			double dLowPrice;
			double dMeanPrice;
			double dVolume;
			double dStarVolume;
		};



}