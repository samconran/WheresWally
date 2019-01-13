#include "Large_Image.h"

//Large_Image constructor, taking in a filename and passing it on to the base constructor
Large_Image::Large_Image(string filename) :Base_Img(filename)
{

}

//Large_Image copy constructor, deep copies values from the original to the new object
Large_Image::Large_Image(const Large_Image &original)
{
	columns = original.columns;
	rows = original.rows;
	content = original.content; //Vectors' assignment operator automatically does a deep copy
}

/* 
The CompareRef method initiates the search through the large image, going through each pixel
and treating it as the "start point" for generating another ref image and calculating the
ZNCC value between the two. It keeps track of the n-best list of matches.
The method is passed:
 - A template image (a pointer to a Ref_Image)
 - The results list (a pointer to a vector of pairs)
*/
void Large_Image::CompareRef(Ref_Image* templateRef, vector<pair<int, double>>* results)
{
	//Read the number of rows/columns from the template reference image into variables
	//Initially in order to avoid constant function calls to get the values
	int templateRows = templateRef->getRows();
	int templateColumns = templateRef->getColumns();
	
	//For every row in the large image...
	for (int largeRow = 0; largeRow < rows; largeRow++)
	{
		//If the template is going to "overflow" the rows of the large image, don't try to perform the zncc algorithm
		if (rows - largeRow < templateRows)
			continue;

		//For every column in the large image...
		for (int largeColumn = 0; largeColumn < columns; largeColumn++)
		{
			//If the template is going to "overflow" the columns of the large image, don't try to perform the zncc algorithm
			if (columns - largeColumn < templateColumns)
				continue;

			//Work out the 1D array position of the start location
			int start = largeColumn + largeRow * columns;

			//Create a new reference image from this location of the same size as the template image
			unique_ptr<Ref_Image> searchTile = createRef(start, templateRows, templateColumns);

			//Work out the ZNCC value, comparing this new search tile and the template image (wally)
			double zncc = (*templateRef) * (*searchTile);

			//If the ZNCC result is greater (i.e. a better match) than the last element in the (already sorted) results vector...
			if (zncc > results->at(results->size() - 1).second)
			{
				//...Set the last element (the worst match) in the vector to the new (better) match...
				results->at(results->size() - 1) = pair<int, double>{ start, zncc };

				//...then reverse sort (descending order) the vector by the zncc results to put the new value in the right place
				//using a generalised lambda expresion (available as of C++14).
				sort(results->rbegin(), results->rend(), [](auto &l, auto &r) {
					return l.second < r.second;
				});
			}

			//smart pointer to searchTile refImage handles releasing the memory, so no need to delete it manually here
		}
	}
}

/*
createRef takes a position in the large image and the desired size of a new reference image, and creates one from that position.
It then returns a pointer to this new reference image.
It takes in:
- start (the index of the starting location in the large image to create the new ref from
- refRows (The number of rows to give the new reference image)
- refColumns (The number of columns to give the new reference image)
*/
unique_ptr<Ref_Image> Large_Image::createRef(int start, int refRows, int refColumns)
{
	//Create a new reference image using the (int, int) overload of the Ref_Image constructor
	unique_ptr<Ref_Image> newRef(new Ref_Image(refRows, refColumns));
	
	//For every row and column of a ref image...
	for (int rR = 0; rR < refRows; rR++)
		for (int rC = 0; rC < refColumns; rC++)
			//Add the value at the corresponding pixel location in the large image
			(*newRef) += (content.at(start + (rR * columns) + rC));

	//Then calculate the mean and SD of this new reference image and return the pointer
	newRef->calculateMean();
	newRef->calculateSD();
	return newRef;
}

/*
outline takes in a position in the large image and the desired size of the tile to outline, and draws said outline
The parameters are:
- startPosition (the index of the starting position to draw from)
- tileRows (The number of rows to draw an outline round)
- tileColumns (The number of columns to draw an outline round)
*/
void Large_Image::outline(int startPosition, int tileRows, int tileColumns)
{
	//For all rows/columns specified...
	for (int row = 0; row < tileRows; row++)
		for (int col = 0; col < tileColumns; col++)
		{
			//If the pixel does not exist within a 3 pixel weighted border on the tile, skip to the next loop iteration...
			if (row > 2 && row < tileRows - 3 && col > 2 && col < tileColumns - 3)
				continue;
			//...otherwise, grab the pixel position in the 1D vector and set this value to 0 (colouring it black)
			int p = startPosition + (row * columns) + col;
			content.at(p) = 0;
		}
}