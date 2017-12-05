#pragma once
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "PhotoDAO.h"
#include "User.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

class ImageManager
{
public:
	//static string FILE_EXTENSION;

	ImageManager(PhotoDAO* photoDAO, const string& directoryPath = PhotoDAO::PHOTO_DIR, Size goalSize = Size(92, 112), bool keepAspectRatio = true)
		:directoryPath(directoryPath), goalSize(goalSize), keepAspectRatio(keepAspectRatio), photoDAO(photoDAO) {
		//if the user gave a directory which does not end with a slash, add one
		//if (this->directoryPath.size() > 0 && (*(--this->directoryPath.end()) != '/' && *(--this->directoryPath.end()) == '\\'))
		//	this->directoryPath.append("/");
	}
	~ImageManager();

	Mat processImage(Mat& image);

	//bool processAndSaveImage(Mat& image, const string& username, const string& filename);
	bool processAndSaveImage(Mat& image, User* user);

	Mat tan_triggs_preprocessing(InputArray src,
		float alpha = 0.1, float tau = 10.0, float gamma = 0.2, int sigma0 = 1,
		int sigma1 = 2);

private:
	string directoryPath;
	Size goalSize;
	bool keepAspectRatio;

	PhotoDAO* photoDAO;	
};

