#include "opencv2/core.hpp"
#include "opencv2/face.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/objdetect.hpp"

#include "ImageManager.h"
#include "FaceRecognitionManager.h"
#include "UserDAO.h"
#include "PhotoDAO.h"
#include "App.h"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
using namespace face;

/** Function Headers */
void detectAndDisplay(Mat frame);
Mat processImage(Mat frame);
void savePhoto(Mat frame);

/** Global variables */
String face_cascade_name = "D:/OpenCV/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "D:/OpenCV/opencv/sources/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";
RNG rng(12345);
String filename = "face.png";
String filename_gray = "face_gray.png";
int filenumber = 0;
int SIZE = 100;

/** @function main */
int main(int argc, const char** argv)
{
	VideoCapture capture;
	Mat frame;
	App* app = new App();
	eyes_cascade.load(eyes_cascade_name);
	face_cascade.load(face_cascade_name);
	int c;
	if (capture.open(0))
	{
		do
		{
			capture.read(frame);
			//-- 3. Apply the classifier to the frame
			if (!frame.empty())
			{
				detectAndDisplay(frame);
			}
			else
			{
				printf(" --(!) No captured frame -- Break!"); break;
			}
			c = waitKey(10);
			switch (c) {
				case 'a':
					capture.read(frame);
					app->savePhoto(frame);

					break;

//				case 'w':
//					userDAO->write_csv();
//					break;

				case 't':
					app->trainRecognizer();
					cout << "trained";
					break;

				case 'e':
					app->test();
					break;

				case 'p':
					capture.read(frame);
					app->predict(frame);
					break;
			}
		} while ((char)c != 'c');
	}
	delete app;
	return 0;
}

Mat processImage(Mat frame) 
{
	Mat frame_result;

	cvtColor(frame, frame_result, CV_BGR2GRAY);
	equalizeHist(frame_result, frame_result);

	return frame_result;
}

/** @function detectAndDisplay */
void detectAndDisplay(Mat frame)
{
	vector<Rect> faces;
	Mat frame_result;

	frame_result = processImage(frame);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_result, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		//Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);
		//ellipse(frame, center, Size(faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		Point a(faces[i].x, faces[i].y);
		Point b(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
		rectangle(frame, a, b, Scalar(11, 215, 18), 2, 8, 0);

		Mat faceROI = frame_result(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point center(faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
	}
	//-- Show what you got
	imshow(window_name, frame);
}

void savePhoto(Mat frame) {

	vector<Rect> faces;
	Mat crop;
	Mat gray;
	Mat res;

	// Set Region of Interest
	cv::Rect roi_b;
	cv::Rect roi_c;

	size_t ic = 0; // ic is index of current element
	int ac = 0; // ac is area of current element

	size_t ib = 0; // ib is index of biggest element
	int ab = 0; // ab is area of biggest element

	face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, Size(30, 30));

	for (ic = 0; ic < faces.size(); ic++) // Iterate through all current elements (detected faces)

	{
		roi_c.x = faces[ic].x;
		roi_c.y = faces[ic].y;
		roi_c.width = (faces[ic].width);
		roi_c.height = (faces[ic].height);

		ac = roi_c.width * roi_c.height; // Get the area of current element (detected face)

		//roi_b.x = faces[ib].x;
		//roi_b.y = faces[ib].y;
		//roi_b.width = (faces[ib].width);
		//roi_b.height = (faces[ib].height);

		//ab = roi_b.width * roi_b.height; // Get the area of biggest element, at beginning it is same as "current" element

		//if (ac > ab)
		//{
		//	ib = ic;
		//	roi_b.x = faces[ib].x;
		//	roi_b.y = faces[ib].y;
		//	roi_b.width = (faces[ib].width);
		//	roi_b.height = (faces[ib].height);
		//}

		crop = frame(roi_c);
		resize(crop, res, Size(SIZE, SIZE), 0, 0, INTER_LINEAR); // This will be needed later while saving images
		cvtColor(crop, gray, CV_BGR2GRAY); // Convert cropped image to Grayscale

										   // Form a filename
		String name = "";
		stringstream ssfn;

		ssfn << filenumber << filename;
		name = ssfn.str();
		imwrite(name, crop);
		ssfn = std::stringstream();
		ssfn << filenumber << filename_gray;
		name = ssfn.str();
		imwrite(name, gray);

		filenumber++;
	}


	// Show image
	if (!crop.empty())
	{
		imshow("detected", res);
		waitKey(0);
	}
	else
		destroyWindow("detected");
}
