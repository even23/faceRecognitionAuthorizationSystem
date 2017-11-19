#pragma once

#include <iostream>
#include <stdio.h>

using namespace std;

bool stringToBool(std::string var)
{
	if (var == "true")
		return true;
	else
		return false;
	throw exception("bad string");

}

string boolToString(bool var)
{
	if (var == 1)
		return "true";
	else
		return "false";
	throw exception("bad string");

}