#include "PhotoDAO.h"

string PhotoDAO::PHOTO_DIR = "images\\";
string PhotoDAO::PHOTO_FILENAME = "database\\photo.csv";

PhotoDAO::PhotoDAO()
{
	photos = new vector<Photo*>();
	filename = PHOTO_FILENAME;
	isModified = false;
	read_csv();
	nextPhotoId = 1;
	if (photos->size() > 0) {
		nextPhotoId = photos->back()->getId() + 1;
	}
}


PhotoDAO::~PhotoDAO()
{
	write_csv();
	delete photos;
}

void PhotoDAO::read_csv(char separator) {
	ifstream file(filename.c_str());
	//if (!file) {
	//	string error_message = "No valid input file was given, please check the given filename.";
	//	CV_Error(Error::StsBadArg, error_message);
	//}
	string line;
	string id;
	string userId;
	getline(file, headers);
	while (getline(file, line)) {
		stringstream liness(line);
		getline(liness, id, separator);
		getline(liness, userId, separator);
		photos->push_back(new Photo(atoi(id.c_str()), atoi(userId.c_str())));
	}
	file.close();
}

void PhotoDAO::write_csv(char separator)
{
	ofstream file(filename);
	file << headers << endl;
	for (vector<Photo*>::iterator photo = photos->begin(); photo != photos->end(); ++photo) {
		file << getCsvRow(**photo, separator) << endl;
	}
	file.close();
}

string PhotoDAO::getCsvRow(Photo photo, char separator)
{
	stringstream result;
	result << photo.getId() << separator
		<< photo.getUserId();
	return result.str();
}

//void PhotoDAO::addPhoto(int userId)
//{
//	photos->push_back(new Photo(nextPhotoId++, userId));
//}

string PhotoDAO::addPhoto(User* user)
{
	photos->push_back(new Photo(nextPhotoId, user->getId()));
	user->addPhoto();
	return to_string(nextPhotoId++) + Photo::FILE_EXTENSION;
}
