/*
 * StringOperations.cpp
 *
 *  Created on: 3 Nov 2013
 *      Author: ieuan
 */

#include "StringOperations.h"
#include <string>
#include <algorithm>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/erase.hpp>
#include "Exceptions.h"
using namespace boost;
using namespace std;

StringOperations::StringOperations() {
}

map<string, string> StringOperations::MapStrings(const vector<string>&elements, const char seperator){

	map<string, string> ret_map;
	for (vector<string>::const_iterator it=elements.begin(); it != elements.end(); ++it)
	{
		int pos;
		string line = *it;
		if ((pos = line.find(seperator)) > 0)
		{
			string key = line.substr(0, pos);
			string val = line.substr(pos+1);

			Trim(key);
			Trim(val);

			std::transform(key.begin(), key.end(), key.begin(), ::tolower);
			std::transform(val.begin(), val.end(), val.begin(), ::tolower);

			ret_map[key] = val;
		}
	}
	return ret_map;
}

int StringOperations::CharToInt(const char* cp)
{
	char to_convert = *cp;
	int ival = (int)to_convert;
	return ival - (int)'0';
}

bool StringOperations::Isset(const char* cp)
{
	return *cp != '\0';
}


string StringOperations::Trim(string& string)
{
  string::size_type pos = string.find_first_of('\r');

  if(pos != string::npos)
	  string.erase(pos);

  pos = string.find_last_not_of(' ');

  if(pos != string::npos) {

	  string.erase(pos + 1);
	  pos = string.find_first_not_of(' ');

	  if(pos != string::npos)
		  string.erase(0, pos);
  }
  else string.erase(string.begin(), string.end());

  return string;
}

bool StringOperations::Contains(const string &s, const char *c)
{
	return s.find(c) != string::npos;
}

string StringOperations::GetFirstLine(const string &s)
{
	stringstream ss(s);
	string ret_val;
	if (getline(ss, ret_val))
		return ret_val;
	else
		throw runtime_error("Invalid input");
}

vector<string> StringOperations::Split(const string &s, char delim){

	stringstream ss(s);
	vector<string>ret;
	string temp;

	while (getline(ss, temp, delim))
	{
		ret.push_back(temp);
	}
	return ret;
}

int StringOperations::StringToInt(const string& str)
{
	return lexical_cast<int>(str);
}

string StringOperations::IntToString(int i)
{
	return lexical_cast<string>(i);
}

StringOperations::~StringOperations() {
	// TODO Auto-generated destructor stub
}
