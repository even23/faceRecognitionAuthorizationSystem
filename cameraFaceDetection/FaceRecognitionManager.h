#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/face.hpp"

#include "UserDAO.h"
#include "PhotoDAO.h"

using namespace std;
using namespace cv;
using namespace face;

#pragma once
class FaceRecognitionManager
{
public:
	static string TRAINING_FILENAME;

	FaceRecognitionManager(UserDAO* userDao, PhotoDAO* photoDao);
	~FaceRecognitionManager();

	void prepareTrainingExamples();
	
	vector<Mat>* getImages() { return images; }
	vector<int>* getLabels() { return labels; }
	UserDAO* getUserDAO() { return userDAO; }
	PhotoDAO* getPhotoDAO() { return photoDAO; }
	void trainRecognizer();
	int predict(Mat);
	void read_csv(vector<Mat>& images, vector<int>& labels, const string& filename = TRAINING_FILENAME, char separator = ';');

private:
	vector<Mat>* images;
	vector<int>* labels;
	UserDAO *userDAO;
	PhotoDAO *photoDAO;
	Ptr<EigenFaceRecognizer> eigenFaceRecognizer;
};

