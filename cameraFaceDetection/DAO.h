#pragma once
using namespace std;

class DAO
{
public:
	virtual void read_csv(char separator = ';') = 0;
	virtual void write_csv(char separator = ';') = 0;

protected:
	bool isModified;
	string filename;
	string headers;
};