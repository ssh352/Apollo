#include "stdafx.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
	char *pp[10];

	pp[] = GetInstrumentList("long.csv");

	void add();
	printf("Hello,World.");
	
	return 0;
}

vector<string> GetInstrumentList(string files)
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

