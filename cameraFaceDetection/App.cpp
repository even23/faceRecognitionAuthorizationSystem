#include "App.h"



App::App()
{
	userDao = new UserDAO();
	photoDao = new PhotoDAO();
	faceRecognitionManager = new FaceRecognitionManager(userDao, photoDao);
}


App::~App()
{
	delete faceRecognitionManager;
	delete photoDao;
	delete userDao;
}
