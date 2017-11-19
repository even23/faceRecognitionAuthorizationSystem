#include "ImageTrainer.h"

ImageTrainer::~ImageTrainer()
{
}

Mat ImageTrainer::processImage(Mat& image)
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

bool ImageTrainer::processAndSaveImage(Mat& image, const string& username, const string& filename)
{
	//if(username.exist)
	string path = directoryPath + username;
	string folderCreateCommand = "mkdir " + path;

	system(folderCreateCommand.c_str());

	Mat processed = processImage(image);

	return imwrite(path + "\\" + filename +".png", processed);
}