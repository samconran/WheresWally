#pragma once
#include "Base_Img.h"
#include "Ref_Image.h"
#include <algorithm>

class Large_Image : public Base_Img
{
public:
	Large_Image(string = "Cluttered_scene.txt");
	Large_Image(const Large_Image&);
	void CompareRef(Ref_Image*, vector<pair<int, double>>*);
	unique_ptr<Ref_Image> createRef(int, int, int);
	void outline(int, int, int);
};
