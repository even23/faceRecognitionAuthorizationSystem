#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/face.hpp"

#include "UserDAO.h"
#include "PhotoDAO.h"

using namespace std;
using namespace cv;

#pragma once
class FaceRecognitionManager
{
public:
	FaceRecognitionManager(UserDAO* userDao, PhotoDAO* photoDao);
	~FaceRecognitionManager();

private:
	vector<Mat> images;
	vector<int> labels;
	UserDAO *userDAO;
	PhotoDAO *photoDAO;
};

