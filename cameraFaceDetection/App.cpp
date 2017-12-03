#include "App.h"



App::App()
{
	userDao = new UserDAO();
	photoDao = new PhotoDAO();
	imageManager = new ImageManager(photoDao);
	faceDetectionManager = new FaceDetectionManager();
	faceRecognitionManager = new FaceRecognitionManager(userDao, photoDao, imageManager);
	activeUser = &*(userDao->getUsers()->begin());
}


App::~App()
{
	delete faceRecognitionManager;
	delete faceDetectionManager;
	delete imageManager;
	delete photoDao;
	delete userDao;
}

bool App::savePhoto(Mat frame)
{
	vector<Rect>* faces = faceDetectionManager->detectFaces(frame);

	if (faceDetectionManager->getFaces()->size() == 0)
	{
		cout << "No faces found. The detector did not find any faces!";
		return false;
	}
	else
	{
		frame = frame.clone();
		Mat faceRegion;
		for (vector<Rect>::iterator face = faces->begin(); face != faces->end(); ++face)
		{
			faceRegion = frame(*face);
			if (!imageManager->processAndSaveImage(faceRegion, activeUser)) {
				cout << "Failed. Could not process and save the image!";
				return false;
			}
			//faceRecognitionManager->prepareTrainingExample();
		}
	}
	return true;
}

void App::trainRecognizer()
{
	faceRecognitionManager->trainRecognizer();
}

bool App::predict(Mat frame)
{
	Mat faceRegion;
	vector<Rect>* faces = faceDetectionManager->detectFaces(frame);
	if (faceDetectionManager->getFaces()->size() == 0)
	{
		cout << "No faces found. The detector did not find any faces!";
		return false;
	}
	else
	{
		frame = frame.clone();
		for (vector<Rect>::iterator face = faces->begin(); face != faces->end(); ++face)
		{
			faceRegion = frame(*face);
			faceRegion = imageManager->processImage(faceRegion);
		}
	}

	cout << faceRecognitionManager->predict(faceRegion);
	return false;
}
