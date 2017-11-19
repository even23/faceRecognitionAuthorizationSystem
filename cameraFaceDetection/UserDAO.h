#pragma once
#include "User.h"
#include "DAO.h"
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class UserDAO : public DAO
{
public:
	string USER_FILENAME = "database\\user.csv";
	UserDAO();
	~UserDAO();
	void read_csv(char separator = ';');
	void write_csv(char separator = ';');
	string getCsvRow(User user, char separator);

	vector<User>* getUsers() { return users; };

protected:
	//bool isModified;
	//string filename;
	vector<User>* users;
	//string headers;
};

