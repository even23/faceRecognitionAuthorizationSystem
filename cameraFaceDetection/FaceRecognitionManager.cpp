#include "FaceRecognitionManager.h"

string FaceRecognitionManager::TRAINING_FILENAME = "database\\training.csv";

FaceRecognitionManager::FaceRecognitionManager(UserDAO * userDao, PhotoDAO * photoDao)
{
	userDAO = userDao;
	photoDAO = photoDao;
	images = new vector<Mat>();
	labels = new vector<int>();
	//read_csv(*images, *labels);
	eigenFaceRecognizer = EigenFaceRecognizer::create(10, DBL_MAX);
	prepareTrainingExamples();
}

FaceRecognitionManager::~FaceRecognitionManager()
{
	delete images;
	delete labels;
	delete eigenFaceRecognizer;
}

void FaceRecognitionManager::prepareTrainingExamples()
{
	vector<Photo*>* photos = getPhotoDAO()->getPhotos();

	for (vector<Photo*>::iterator photo = photos->begin(); photo != photos->end(); ++photo) {
		prepareTrainingExample(*photo);
	}
}

void FaceRecognitionManager::prepareTrainingExample(Photo* photo)
{
	images->push_back(imread(photo->getDirectory(), CV_LOAD_IMAGE_GRAYSCALE));
	labels->push_back(photo->getUserId());
}

void FaceRecognitionManager::trainRecognizer()
{
	eigenFaceRecognizer->train(*images, *labels);
}

int FaceRecognitionManager::predict(Mat image)
{
	int predicted_label = -1;
	double predicted_confidence = 0.0;
	eigenFaceRecognizer->predict(image, predicted_label, predicted_confidence);
	return predicted_label;
}

void FaceRecognitionManager::read_csv(vector<Mat>& images, vector<int>& labels, const string & filename, char separator)
{
	ifstream file(filename.c_str(), ifstream::in);
	if (!file) {
		string error_message = "No valid input file was given, please check the given filename.";
		CV_Error(Error::StsBadArg, error_message);
	}
	string line, path, classlabel;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			images.push_back(imread(path, 0));
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}