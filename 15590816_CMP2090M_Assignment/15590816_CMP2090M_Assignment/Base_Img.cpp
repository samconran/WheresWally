#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "Base_Img.h"

//Default constructor (utilised by Ref_Image undercertain circumstances) just creates null row/colum ints and creates an empty content vector
Base_Img::Base_Img()
{

}

//Overridden constructor performs the same tasks as above, but takes in the file name and executes readFromFile on it
Base_Img::Base_Img(string file_name)
{
	readFromFile(file_name);
}

//Method to read in the data from a file
void Base_Img::readFromFile(string filename)
{
	//Initialise row and column properties as 0
	columns = 0;
	rows = 0;

	//Set up file input stream
	ifstream myFile(filename);

	//If it's opened sucessfully...
	if (myFile.is_open())
	{
		//Set up string
		string str;
		//While the end of the file hasn't been reached...
		while (!getline(myFile, str).eof())
		{
			//Increment the rows property
			rows++;
			//Set up value double
			double value;
			//set up an input string stream from str
			istringstream r(str);
			//Push in all values from the string into content
			while (r >> value)
				content.push_back(value);
		}
		//Close the file
		myFile.close();
		//Work out the number of columns by dividing the area by the number of rows
		columns = content.size() / rows;
	}
}

/*
Largely unmodified method to write to a .pgm file. the only modification is to take the data/rows/columns from the object properties,
and to hardcode Q in as 255 (still avoiding magic numbers, but setting it in the write function, rather than passing it as a parameter.
*/
void Base_Img::writePGMToFile(string filename)
{
	int i;
	int Q = 255;
	unsigned char *image;
	ofstream myfile;

	image = (unsigned char *) new unsigned char[rows*columns];

	// convert the integer values to unsigned char

	for (i = 0; i < rows*columns; i++)
		image[i] = (unsigned char)content[i];

	myfile.open(filename, ios::out | ios::binary | ios::trunc);

	if (!myfile) {
		cout << "Can't open file: " << filename << endl;
		exit(1);
	}

	myfile << "P5" << endl;
	myfile << columns << " " << rows << endl;
	myfile << Q << endl;

	myfile.write(reinterpret_cast<char *>(image), (rows*columns) * sizeof(unsigned char));

	if (myfile.fail()) {
		cout << "Can't write image " << filename << endl;
		exit(0);
	}

	myfile.close();

	delete[] image;
}