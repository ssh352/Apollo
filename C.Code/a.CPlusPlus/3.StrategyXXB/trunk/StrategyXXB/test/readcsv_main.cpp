#include <iostream>  
#include <math.h>
#include <string> 
#include <time.h>
#include <vector>  
#include <fstream>  
#include <sstream>  

using namespace std;

vector<string> GetLongList(string files);
bool DoTrade(string date);
time_t StringToDatetime(string str);

int main()
{
	time_t tm_list;
	time_t tm_now;

	string date_now = "2017/11/7";

	string longfile = "long.csv";
	vector<string> longlist;

	longlist = GetLongList(longfile);

	tm_list = StringToDatetime(longlist[0]);

	cout << longlist[0] << endl;

	tm_now = StringToDatetime(date_now);

	cout << date_now << endl;

	double ms = difftime(tm_now,tm_list);

	double days = ceil(ms / 86400);

	cout << days << endl;

	for (int i = 0; i < longlist.size(); i++)
	{
		cout << longlist[i] << endl;
	}

	getchar();

	return 0;
}

vector<string> GetLongList(string files)
{
	string str;
	string lineStr;
	stringstream ss;

	vector<string> lineArray;
	ifstream inFile(files, ios::in);

	while (getline(inFile, lineStr))
	{  	
		ss.str(lineStr);
	}

	while (getline(ss, str, ','))
		lineArray.push_back(str);

	return lineArray;
}

bool DoTrade(string date_now)
{
	time_t tm_list;
	time_t tm_now;
	string lastdate;
	string longfile = "long.csv";
	vector<string> longlist;

	longlist = GetLongList(longfile);
	lastdate = longlist[0];

	tm_list = StringToDatetime(longlist[0]);
	tm_now = StringToDatetime(date_now);


	double ms = difftime(tm_now, tm_list);

	double days = ceil(ms / 86400);

	if (days >= 5)
		return true;
	else
		return false;
}

time_t StringToDatetime(string str)
{
	char *cha = (char*)str.data();            
	tm tm_;                                   
	int year, month, day, hour = 0, minute = 0, second = 0;
	sscanf(cha, "%d/%d/%d ", &year, &month, &day);
	tm_.tm_year = year - 1900;                
	tm_.tm_mon = month - 1;                   
	tm_.tm_mday = day;
	tm_.tm_hour = hour;                       
	tm_.tm_min = minute;                      
	tm_.tm_sec = second;
	tm_.tm_isdst = 0;                         
	time_t t_ = mktime(&tm_);                 
	return t_;                                
}

