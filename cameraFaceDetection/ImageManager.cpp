#include "ImageManager.h"

//string ImageTrainer::FILE_EXTENSION = ".pgm";

ImageManager::~ImageManager()
{
}

Mat ImageManager::processImage(Mat& image)
{
	Mat resized;
	Size originalSize = image.size();

	if (keepAspectRatio)
	{
		float ratio = static_cast<float>(goalSize.height) / originalSize.height;
		Size newSize((int)(originalSize.width * ratio), (int)(originalSize.height * ratio));

		//fix possible rounding error by float
		if (newSize.height != goalSize.height) newSize.height = goalSize.height;

		resize(image, resized, newSize);

		if (resized.size().width != goalSize.width)
		{
			if (keepAspectRatio)
			{
				int delta = goalSize.width - resized.size().width;

				if (delta < 0)
				{
					Rect clipRect(std::abs(delta) / 2, 0, goalSize.width, resized.size().height);
					resized = resized(clipRect);
				}
				else if (delta > 0)
				{
					//width needs to be widened, create bigger mat, get region of 
					//interest at the center that matches the size of the resized   
					//image, and copy the resized image into that ROI

					Mat widened(goalSize, resized.type());
					Rect widenRect(delta / 2, 0, goalSize.width, goalSize.height);
					Mat widenedCenter = widened(widenRect);
					resized.copyTo(widenedCenter);
					resized = widened; //we return resized, so set widened to resized
				}
			}
		}
	}
	else
		resize(image, resized, goalSize);

	Mat grayFrame;
	cvtColor(resized, grayFrame, CV_BGR2GRAY);
	equalizeHist(grayFrame, grayFrame);
	//TODO: better processing in here

	return grayFrame;
}

//bool ImageTrainer::processAndSaveImage(Mat& image, const string& userId, const string& filename)
//{
//	//if(username.exist)
//	string path = directoryPath + userId;
//	string folderCreateCommand = "mkdir " + path;
//
//	system(folderCreateCommand.c_str());
//
//	Mat processed = processImage(image);
//
//	photoDAO->addPhoto(atoi(userId.c_str()));
//
//	return imwrite(path + "\\" + filename + Photo::FILE_EXTENSION, processed);
//}

bool ImageManager::processAndSaveImage(Mat& image, User* user)
{
	//if(username.exist)
	int userId = user->getId();
	string path = directoryPath + to_string(userId);
	string folderCreateCommand = "mkdir " + path;

	system(folderCreateCommand.c_str());

	Mat processed = processImage(image);

	string filename = photoDAO->addPhoto(user);
	user->addPhoto();

	return imwrite(path + "\\" + filename, processed);
}