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
	if (resized.channels() != 1)
		cvtColor(resized, grayFrame, CV_BGR2GRAY);
	else
		grayFrame = resized;
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

Mat ImageManager::tan_triggs_preprocessing(InputArray src,
	float alpha, float tau, float gamma, int sigma0,
	int sigma1) {

	// Convert to floating point:
	Mat X = src.getMat();
	X.convertTo(X, CV_32FC1);
	// Start preprocessing:
	Mat I;
	pow(X, gamma, I);
	// Calculate the DOG Image:
	{
		Mat gaussian0, gaussian1;
		// Kernel Size:
		int kernel_sz0 = (3 * sigma0);
		int kernel_sz1 = (3 * sigma1);
		// Make them odd for OpenCV:
		kernel_sz0 += ((kernel_sz0 % 2) == 0) ? 1 : 0;
		kernel_sz1 += ((kernel_sz1 % 2) == 0) ? 1 : 0;
		GaussianBlur(I, gaussian0, Size(kernel_sz0, kernel_sz0), sigma0, sigma0, BORDER_REPLICATE);
		GaussianBlur(I, gaussian1, Size(kernel_sz1, kernel_sz1), sigma1, sigma1, BORDER_REPLICATE);
		subtract(gaussian0, gaussian1, I);
	}

	{
		double meanI = 0.0;
		{
			Mat tmp;
			pow(abs(I), alpha, tmp);
			meanI = mean(tmp).val[0];

		}
		I = I / pow(meanI, 1.0 / alpha);
	}

	{
		double meanI = 0.0;
		{
			Mat tmp;
			pow(min(abs(I), tau), alpha, tmp);
			meanI = mean(tmp).val[0];
		}
		I = I / pow(meanI, 1.0 / alpha);
	}

	// Squash into the tanh:
	{
		Mat exp_x, exp_negx;
		exp(I / tau, exp_x);
		exp(-I / tau, exp_negx);
		divide(exp_x - exp_negx, exp_x + exp_negx, I);
		I = tau * I;
	}
	return I;
}