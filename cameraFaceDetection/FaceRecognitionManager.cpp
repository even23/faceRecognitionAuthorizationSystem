#include "FaceRecognitionManager.h"

string FaceRecognitionManager::TRAINING_FILENAME = "database\\training.csv";

FaceRecognitionManager::FaceRecognitionManager(UserDAO * userDao, PhotoDAO * photoDao, ImageManager *_imageManager)
{
	userDAO = userDao;
	photoDAO = photoDao;
	imageManager = _imageManager;
	images = new vector<Mat>();
	labels = new vector<int>();
	read_csv(*images, *labels);
	eigenFaceRecognizer = EigenFaceRecognizer::create(100, DBL_MAX);
	prepareTrainingExamples();
	fisherFaceRecognizer = FisherFaceRecognizer::create();
	LBPHFaceRecognizer = LBPHFaceRecognizer::create(1, 8, 8, 8, DBL_MAX);
}

FaceRecognitionManager::~FaceRecognitionManager()
{
	delete images;
	delete labels;
//	delete eigenFaceRecognizer;
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
	fisherFaceRecognizer->train(*images, *labels);
	LBPHFaceRecognizer->train(*images, *labels);

}

int FaceRecognitionManager::predict(Mat image)
{
	int predicted_label = -1;
	double predicted_confidence = 0.0;
	Ptr<PredictCollector> collector1 = new StandardCollector();
	Ptr<PredictCollector> collector2 = new StandardCollector();
	Ptr<PredictCollector> collector3 = new StandardCollector();
	//eigenFaceRecognizer->predict(image, predicted_label, predicted_confidence);
	//fisherFaceRecognizer->predict(image, predicted_label, predicted_confidence);
	//LBPHFaceRecognizer->predict(image, predicted_label, predicted_confidence);
	eigenFaceRecognizer->predict(image, collector1);
	fisherFaceRecognizer->predict(image, collector2);
	LBPHFaceRecognizer->predict(image, collector3);

	//Mat eigenvalues = eigenFaceRecognizer->getEigenValues();
	//// And we can do the same to display the Eigenvectors (read Eigenfaces):
	//Mat W = eigenFaceRecognizer->getEigenVectors();
	//// Get the sample mean from the training data
	//Mat mean = eigenFaceRecognizer->getMean();
	//int height = images->at(0).rows;
	//imshow("mean", norm_0_255(mean.reshape(1, height)));

	//EigenFaces
	//// Display or save the Eigenfaces:
	//for (int i = 0; i < min(10, W.cols); i++) {
	//	string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
	//	cout << msg << endl;
	//	// get eigenvector #i
	//	Mat ev = W.col(i).clone();
	//	// Reshape to original size & normalize to [0...255] for imshow.
	//	Mat grayscale = norm_0_255(ev.reshape(1, height));
	//	// Show the image & apply a Jet colormap for better sensing.
	//	Mat cgrayscale;
	//	applyColorMap(grayscale, cgrayscale, COLORMAP_JET);
	//	// Display or save:
	//	imshow(format("eigenface_%d", i), cgrayscale);
	//}

	//// Display or save the image reconstruction at some predefined steps:
	//for (int num_components = min(W.cols, 10); num_components < min(W.cols, 300); num_components += 15) {
	//	// slice the eigenvectors from the model
	//	Mat evs = Mat(W, Range::all(), Range(0, num_components));
	//	Mat projection = LDA::subspaceProject(evs, mean, images->at(0).reshape(1, 1));
	//	Mat reconstruction = LDA::subspaceReconstruct(evs, mean, projection);
	//	// Normalize the result:
	//	reconstruction = norm_0_255(reconstruction.reshape(1, height));
	//	// Display or save:
	//	imshow(format("eigenface_reconstruction_%d", num_components), reconstruction);
	//}



	//// We could get the histograms for example:
	//vector<Mat> histograms = LBPHFaceRecognizer->getHistograms();
	//// But should I really visualize it? Probably the length is interesting:
	//cout << "Size of the histograms: " << histograms[0].total() << endl;

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
			Mat image = imread(path, 0);
			image = imageManager->processImage(image);
			images.push_back(image);
			labels.push_back(atoi(classlabel.c_str()));
		}
	}
}