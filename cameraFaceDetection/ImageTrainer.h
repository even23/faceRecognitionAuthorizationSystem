#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

class ImageTrainer
{
public:
	ImageTrainer(Size goalSize = Size(100, 100), bool keepAspectRatio = true, const string& directoryPath = "")
		:directoryPath(directoryPath), goalSize(goalSize), keepAspectRatio(keepAspectRatio) {
		//if the user gave a directory which does not end with a slash, add one
		//if (this->directoryPath.size() > 0 && (*(--this->directoryPath.end()) != '/' && *(--this->directoryPath.end()) == '\\'))
		//	this->directoryPath.append("/");
	}
	~ImageTrainer();

	Mat processImage(Mat& image);

	bool processAndSaveImage(Mat& image, const string& username, const string& filename);

protected:

	string directoryPath;
	Size goalSize;
	bool keepAspectRatio;
};

