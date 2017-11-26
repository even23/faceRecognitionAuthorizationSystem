#pragma once
#include "Photo.h"
#include "User.h"
#include "DAO.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class PhotoDAO : public DAO
{
public:
	string PHOTO_FILENAME = "database\\photo.csv";
	PhotoDAO();
	~PhotoDAO();

	void read_csv(char separator = ';');
	void write_csv(char separator = ';');
	string getCsvRow(Photo photo, char separator);

	vector<Photo*>* getPhotos() { return photos; }

	void addPhoto(int);
	string addPhoto(User*);

private:
	int nextPhotoId;
	vector<Photo*>* photos;
};