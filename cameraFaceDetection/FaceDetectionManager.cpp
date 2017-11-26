#include "faceDetectionManager.h"

String FaceDetectionManager::FACE_CASCADE_NAME = "D:/OpenCV/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
String FaceDetectionManager::EYES_CASCADE_NAME = "D:/OpenCV/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";


FaceDetectionManager::FaceDetectionManager()
{
	faceCascade = new CascadeClassifier();
	eyesCascade = new CascadeClassifier();
	faceCascade->load(FACE_CASCADE_NAME);
	eyesCascade->load(EYES_CASCADE_NAME);
	faces = new vector<Rect>();
}


FaceDetectionManager::~FaceDetectionManager()
{
	//delete faceCascade;
	//delete eyesCascade;
	delete faces;
}

vector<Rect>* FaceDetectionManager::detectFaces(Mat frame)
{
	faceCascade->detectMultiScale(frame, *faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));
	return faces;
}
