#include "FaceRecognitionManager.h"

#pragma once
class App
{
public:
	App();
	~App();

private:
	UserDAO* userDao;
	PhotoDAO* photoDao;
	FaceRecognitionManager* faceRecognitionManager;
	User* activeUser;

};

