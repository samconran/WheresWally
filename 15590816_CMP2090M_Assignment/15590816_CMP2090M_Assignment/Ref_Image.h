#pragma once
#include "Base_Img.h"

class Ref_Image : public Base_Img
{
private:
	double mean;
	double standardDeviation;
	double ZNCC(const Ref_Image&);
public:
	Ref_Image(string = "Wally_grey.txt");
	Ref_Image(int, int);

	double operator * (const Ref_Image&);
	void operator += (double);

	int getRows();
	int getColumns();

	void calculateMean();
	void calculateSD();
};