#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
using namespace std;
namespace CTPTOOL {
	class BASELIB_API StringTool {

	public:
		static string GetStringUnCludeNumber(string str);
		static vector<string> split(const  string& s, const string& delim);
	};
}