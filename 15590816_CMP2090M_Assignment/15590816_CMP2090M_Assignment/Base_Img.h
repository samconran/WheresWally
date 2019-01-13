#pragma once
#include <iostream>
#include <vector>

using namespace std;

class Base_Img
{
protected:
	int columns;
	int rows;
	vector<double> content;
public:
	Base_Img();
	Base_Img(string);
	void readFromFile(string);
	void writePGMToFile(string);
};