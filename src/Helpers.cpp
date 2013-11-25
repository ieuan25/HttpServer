/*
 * HttpHelpers.cpp
 *
 *  Created on: 29 Sep 2013
 *      Author: ieuan
 */

#include "Helpers.h"
#include <fstream>
#include <stdexcept>
#include <cstring>
#include <iostream>
#include "Exceptions.h"
#include "StringOperations.h"
using namespace std;

Helpers::Helpers()
{

}

string Helpers::GetCurrentTime()
{
	time_t now(0);
    time(&now);
    struct tm * current_time = localtime(&now);
    char * ctime = asctime(current_time);
    string time(ctime);
    size_t found = time.find_first_of('\n');
    if (found != string::npos)
    	time = time.substr(0, found);

    return StringOperations::Trim(time);
}

string Helpers::ReadFile(const char *filename)
{
	ifstream ifs;
	int file_size = 0;
	char * buffer;
	string ret;

	ifs.open(filename);

	if (ifs.good())
	{
		ifs.seekg(0, ifs.end);
		file_size = ifs.tellg();

		if (file_size < 0)
			throw file_not_found("Resource not found");

		ifs.seekg(0, ifs.beg);
		buffer = new char[file_size];
		ifs.read(buffer, file_size);
		CopyBufferToString(ret, buffer, file_size);
		delete buffer;
	}
	else
	{
		throw file_not_found("Resource could not be found.");
	}
	return ret;
}

void Helpers::CopyBufferToString(string &s, char * buffer, int num_bytes)
{
	for (int i = 0; i < num_bytes; i++)
	{
		s += *buffer;
		buffer++;
	}
}

Helpers::~Helpers()
{}

