#include "FaceRecognitionManager.h"

string FaceRecognitionManager::TRAINING_FILENAME = "database\\training.csv";

string getCsvRow(Photo photo, char separator)
{
	stringstream result;
	result << photo.getId() << separator
		<< photo.getUserId();
	return result.str();
}

void FaceRecognitionManager::computeResults(Ptr<StandardCollector> collector1) {
	double min, mediana, threshhold;
	int label, risk, miss;
	vector<pair<int, double>> res = collector1->getResults(true);
	label = res.at(0).first;
	min = res.at(0).second;
	mediana = res.at(res.size() / 2).second;
	for (vector<pair<int, double>>::iterator pair = res.begin(); pair != res.end(); ++pair) {
		if (pair->first != label) {
			threshhold = pair->second;
			risk = pair - res.begin();
			miss = pair->first;
			break;
		}
	}
	file << label << ";" << min << ";" << mediana << ";" << risk << ";" << threshhold << ";" << miss << endl;
}

FaceRecognitionManager::FaceRecognitionManager(UserDAO * userDao, PhotoDAO * photoDao, ImageManager *_imageManager)
{
	userDAO = userDao;
	photoDAO = photoDao;
	imageManager = _imageManager;
	images = new vector<Mat>();
	labels = new vector<int>();

	testImages = new vector<Mat>();
	testLabels = new vector<int>();

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
	Mat image = imread(photo->getDirectory(), CV_LOAD_IMAGE_GRAYSCALE);
	images->push_back(imread(photo->getDirectory(), CV_LOAD_IMAGE_GRAYSCALE));
	labels->push_back(photo->getUserId());
	image = imageManager->tan_triggs_preprocessing(image);
	normalize(image, image, 0.0, 1.0, CV_MINMAX, CV_64F);
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
	Ptr<StandardCollector> collector1 = new StandardCollector();
	Ptr<StandardCollector> collector2 = new StandardCollector();
	Ptr<StandardCollector> collector3 = new StandardCollector();

	// rand from images
	//output = (rand() % static_cast<int>(images->size())) + 1;
	//testImage = new Mat(images->at(output));
	//label = labels->at(output);

	// rand from testimages
	//output = (rand() % static_cast<int>(testImages->size())) + 1;
	//testImage = new Mat(testImages->at(output));
	//label = testLabels->at(output);

	testImage = new Mat(image);

	//preprocessing
//	*testImage = imageManager->tan_triggs_preprocessing(*testImage);
	//normalize(*testImage, *testImage, 0.0, 1.0, CV_MINMAX, CV_64F);
	//Mat noise = Mat(testImage->size(), testImage->type());
	//cv::randn(noise, 0, 0.1);
	//*testImage = *testImage + noise;
	//normalize(*testImage, *testImage, 0.0, 1.0, CV_MINMAX, CV_64F);
	//testImage->convertTo(*testImage, CV_8U, 255, 0);

	eigenFaceRecognizer->predict(*testImage, collector1);
	fisherFaceRecognizer->predict(*testImage, collector2);
	LBPHFaceRecognizer->predict(*testImage, collector3);
	vector<pair<int, double>> res1 = collector1->getResults(true);
	computeResults(collector1);
	vector<pair<int, double>> res2 = collector2->getResults(true);
	computeResults(collector2);
	vector<pair<int, double>> res3 = collector3->getResults(true);
	computeResults(collector3);

	//eigenFaceRecognizer->predict(image, predicted_label, predicted_confidence);
	//fisherFaceRecognizer->predict(image, predicted_label, predicted_confidence);
	//LBPHFaceRecognizer->predict(image, predicted_label, predicted_confidence);

	//eigenFaceRecognizer->predict(image, collector1);
	//fisherFaceRecognizer->predict(image, collector2);
	//LBPHFaceRecognizer->predict(image, collector3);

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
	int a = 0;
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, path, separator);
		getline(liness, classlabel);
		if (!path.empty() && !classlabel.empty()) {
			Mat image = imread(path, 0);
			image = imageManager->processImage(image);
			if (((a % 10) > 2) && ((a % 10) <= 5)) {
				images.push_back(image);
				labels.push_back(atoi(classlabel.c_str()));
			}
			else if (a % 10 == 2) {
				testImages->push_back(image);
				testLabels->push_back(atoi(classlabel.c_str()));
			}
			a++;
		}
	}
}

void FaceRecognitionManager::test()
{
	file.open("test\\test3.csv");
	file << "40x3 + 40x1, no noise, no normalization, no tran_triggs" << endl;
	file << "Wynik;min;mediana;risk;tH;miss" << endl;
	for (vector<Mat>::iterator image = testImages->begin(); image != testImages->end(); ++image) {
		predict(*image);
	}
	file.close();
}
