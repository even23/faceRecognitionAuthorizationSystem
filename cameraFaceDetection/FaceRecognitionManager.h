#pragma once

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/face.hpp"

#include "UserDAO.h"
#include "PhotoDAO.h"
#include "Utils.h"
#include "ImageManager.h"

using namespace std;
using namespace cv;
using namespace face;

class FaceRecognitionManager
{
public:
	static string TRAINING_FILENAME;

	FaceRecognitionManager(UserDAO* userDao, PhotoDAO* photoDao, ImageManager* imageManager);
	~FaceRecognitionManager();

	void prepareTrainingExamples();
	void prepareTrainingExample(Photo* photo);
	
	vector<Mat>* getImages() { return images; }
	vector<int>* getLabels() { return labels; }
	UserDAO* getUserDAO() { return userDAO; }
	PhotoDAO* getPhotoDAO() { return photoDAO; }
	void trainRecognizer();
	int predict(Mat);
	void read_csv(vector<Mat>& images, vector<int>& labels, const string& filename = TRAINING_FILENAME, char separator = ';');

	void test();
	void computeResults(Ptr<StandardCollector> collector1);

	vector<Mat>* testImages;
	vector<int>* testLabels;
	ofstream file;
private:
	vector<Mat>* images;
	vector<int>* labels;
	UserDAO *userDAO;
	PhotoDAO *photoDAO;
	ImageManager *imageManager;
	Ptr<EigenFaceRecognizer> eigenFaceRecognizer;
	Ptr<FisherFaceRecognizer> fisherFaceRecognizer;
	Ptr<LBPHFaceRecognizer> LBPHFaceRecognizer;

	int output;
	Mat *testImage;
	int label;


};

