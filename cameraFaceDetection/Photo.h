#pragma once

#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace std;

class Photo
{
public:
	Photo(int id, int userId) :
		id(id), userId(userId) {
		stringstream ss;
		ss << "images\\" << userId << "\\" << id << ".png";
		directory = ss.str();
	}
	~Photo();

	int getId() { return id; }
	int getUserId() { return userId; }
	string getDirectory() { return directory; }
private:
	int id;
	int userId;
	string directory;
};
