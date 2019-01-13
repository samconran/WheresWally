#include <iostream>
#include <sstream>
#include "Base_Img.h"
#include "Ref_Image.h"
#include "Large_Image.h"

/*---------------------------------------
|		  CMP2090M Assignment 1			|
|	     By Sam Conran (15590816)		|
|										|
|	To ensure this runs in a sensible	|
|	 timeframe, please ensure that it	|	
|	is run in release, rather than in	|
|	debug, mode.						|
|										|
|--------------------------------------*/


//Use the standard namespace
using namespace std;

int main()
{
	//Create the wally & scene objects, as well as the results vector and the n value
	Ref_Image wally;
	Large_Image scene;
	vector<pair<int, double>> results;
	int n = 0;

	//Present the opening menu and get the number of matches (n) from the user
	cout << "<======== Where's Wally: a C++ implementation ========>" << endl << endl;
	cout << "Please enter the number of matches you wish to find in the n-best list:" << endl;
	//data Validation loop
	while (true)
	{
		cin >> n;
		if (!cin || n < 1) // user didn't input a valid number of matches
		{
			//reset failbit
			cin.clear();
			//skip any form of bad input
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			//Present 
			cout << "Please enter a valid number:" << endl;
		}
		else //If good input, carry on
		{
			break;
		}
	}

	//Push n amounts of STL pair structs to the results vector with default values (no index (-1), 0 ZNCC)
	for (int i = 0; i < n; i++)
	{
		results.push_back(pair<int, double>{ -1, 0 });
	}

	//Print out program status
	cout << endl << "Finding the " << n << " best match(es) for wally in the cluttered scene..." << endl;
	cout << "Please be aware that this might take a while." << endl << endl;

	//Call the compare method from large image and pass in pointers to the ref image and the results
	scene.CompareRef(&wally, &results);

	//Print out the results
	cout << "Match:\tZNCC value:" << endl;
	int matchCount = 1;
	for (auto result : results)
	{
		cout << matchCount++ << "\t" << result.second << endl;
	}

	//Declare an int for the number of matches to outline
	int matchOutlines;

	//If the number of matches is greater than 1...
	if (n > 1)
	{
		//...set matchOutlines to 0 and ask the user how many they would like to outline...
		matchOutlines = 0;
		cout << endl << "Out of the " << n << " matches, how many would you like to outline?" << endl;
		while (true)
		{
			cin >> matchOutlines;
			if (!cin || matchOutlines < 1 || matchOutlines > n)
			{
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				cout << "Please enter a valid number that is between 0 and " << n << ":" << endl;
			}
			else
			{
				break;
			}
		}
	}
	//Else, if there is only one match, just set matchOutlines to 1
	else
	{
		matchOutlines = 1;
	}

	//Initialise a boolean value to true, represents whether all outlines should be in the same file.
	bool outlineSameFile = true;

	//If there are more than one matches the user wants to outline...
	if (matchOutlines > 1)
	{
		//...Give the user the option to not do this and print to seperate files
		char userCheck;
		cout << "Would you like to print all the matches in the same file? (if so, type 'Y')" << endl;
		cin >> userCheck;

		outlineSameFile = (tolower(userCheck) == 'y');
	}

	//If all matches should be outlined in the same file...
	if (outlineSameFile)
	{
		//...Set up a smart pointer to a new Large image deep copied (from the copy constructor) from the scene...
		unique_ptr<Large_Image> sceneCopy(new Large_Image(scene));

		//...For all match outlines, call outline on this copy, passing in the corresponding result's start position and a ref image's rows/columns size...
		for (int i = 0; i < matchOutlines; i++)
		{
			sceneCopy->outline(results.at(i).first, wally.getRows(), wally.getColumns());
		}

		//...Set up an output string stream to create the filename with the number of matches outlined...
		ostringstream ss;
		ss << "WheresWallyResult_Best" << matchOutlines << "Matches.pgm";

		//...Write the scene copy to the string value of the string stream defined above...
		sceneCopy->writePGMToFile(ss.str());

		//...a smart pointer is used, so there is no need to call delete on the scene copy here.
	}
	//If the matches should be outlined in different files...
	else
	{
		//...For the number of matches to be outlined...
		for (int i = 0; i < matchOutlines; i++)
		{
			//...Create a new copy of the scene image...
			unique_ptr<Large_Image> sceneCopy(new Large_Image(scene));

			//...outline the search tile (size ref rows by ref colums) at the corrsponding result's start position...
			sceneCopy->outline(results.at(i).first, wally.getRows(), wally.getColumns());

			//...Set up an ouput string stream...
			ostringstream ss;

			//...Set the first part of the string stream up...
			ss << "WheresWallyResult_";

			//...If the best match, add this to the stream...
			if (i == 0)
				ss << "_BestMatch.pgm";
			//...If not, add the number to the filename...
			else
				ss << "_NextBestMatch" << i << ".pgm";

			//...Write the image to the filename defined in the string stream...
			sceneCopy->writePGMToFile(ss.str());

			//...sceneCopy automatically deleted by smart pointer, so there is no need to manually call the delete operator on it here.

		}
	}

	//Output the last lines of the program to the user.
	cout << endl << "The file has been output. Please be aware that many matches may be only a few pixels off from each other, so may appear as an extra thick border around a particular search tile." << endl << endl;
	cout << "Thank you for using this C++ implementation of Where's Wally!" << endl << endl;

	system("pause");
	return 0;
}