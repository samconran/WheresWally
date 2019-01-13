#include "Ref_Image.h"

//Pass in the filename to the base class constructor. After it has been constructed, calculate the mean and standard deviation
Ref_Image::Ref_Image(string filename):Base_Img(filename)
{
	calculateMean();
	calculateSD();
}

//Overload the constructor to take in the rows and columns for the matrix
//An empty content vector will automatically be created
Ref_Image::Ref_Image(int r, int c)
{
	rows = r;
	columns = c;
}

//Overload the * operator to be an abstraction of the ZNCC method
double Ref_Image::operator * (const Ref_Image& other)
{
	return ZNCC(other);
}

//Overload the += operator to add a value to the content matrix
void Ref_Image::operator += (double value)
{
	content.push_back(value);
}

//The ZNCC method takes an object of another reference image and returns the Zero Normalised Cross Correlation value.
double Ref_Image::ZNCC(const Ref_Image& other)
{
	//Initialise the zncc result value as 0 at the start:
	double zncc = 0;

	//For every row in template image...
	for (int tR = 0; tR < rows; tR++)
	{
		//For every column in the template image...
		for (int tC = 0; tC < columns; tC++)
		{
			//Work out 1D position for each ref image...
			int position = tC + tR * columns;
			//...Then take the mean away from each. Multiply respective values from each ref image, then divide by the two standard deviations.
			zncc += ((content.at(position) - mean) * (other.content.at(position) - other.mean)) / (standardDeviation * other.standardDeviation);
		}
	}

	//Then divide this value by the size (area) of the ref images to get the final ZNCC value
	zncc /= rows * columns;

	//return the final ZNCC value.
	return zncc;
}

//Getter function to return the number of rows
int Ref_Image::getRows()
{
	return rows;
}

//Getter function to return the number of columns
int Ref_Image::getColumns()
{
	return columns;
}

//Function to calculate the mean
void Ref_Image::calculateMean()
{
	//Initialise sum to 0
	double sum = 0;

	//For all pixel values in the content matrix...
	for (double pixelVal : content)
		//...Add them to the sum
		sum += pixelVal;

	//Set the mean to the sum divided by the area of the matrix
	mean = sum / content.size();
}

//Function to calculate the standard deviation
void Ref_Image::calculateSD()
{
	//Initialise the sum to 0
	double sum = 0;

	//For all pixel values in the content matrix...
	for (double pixelVal : content)
		//...add the square of the values - the mean to the sum
		sum += (pixelVal - mean) * (pixelVal - mean);

	//Divide the sum by the area of the content matrix
	sum /= content.size();
	//Set the standardDeviation to the sqaure root of this value
	standardDeviation = sqrt(sum);
}