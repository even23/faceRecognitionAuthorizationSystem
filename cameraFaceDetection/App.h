#pragma once

#include "FaceRecognitionManager.h"
#include "FaceDetectionManager.h"
#include "ImageManager.h"

class App
{
public:
	App();
	~App();

	//bool writeCsv();
	bool savePhoto(Mat frame);
	void trainRecognizer();
	bool predict(Mat frame);

private:
	VideoCapture capture;

	UserDAO* userDao;
	PhotoDAO* photoDao;
	FaceRecognitionManager* faceRecognitionManager;
	FaceDetectionManager* faceDetectionManager;
	ImageManager* imageManager;
	User* activeUser;
};

