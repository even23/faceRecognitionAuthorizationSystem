#include "PhotoDAO.h"



PhotoDAO::PhotoDAO()
{
	photos = new vector<Photo>();
	filename = PHOTO_FILENAME;
	isModified = false;
	read_csv();
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
		photos->push_back(Photo(atoi(id.c_str()), atoi(userId.c_str())));
	}
	file.close();
}

void PhotoDAO::write_csv(char separator)
{
	ofstream file(filename);
	file << headers << endl;
	for (vector<Photo>::iterator user = photos->begin(); user != photos->end(); ++user) {
		file << getCsvRow(*user, separator) << endl;
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
