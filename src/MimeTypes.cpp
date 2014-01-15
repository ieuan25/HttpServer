/*
 * MimeTypes.cpp
 *
 *  Created on: 20 Oct 2013
 *      Author: ieuan
 */

#include "MimeTypes.h"
#include <cstring>
#include <string>
#include <iostream>
#include "Helpers.h"
#include "StringOperations.h"

using namespace std;


MimeTypeConf::MimeTypeConf(const char * path) : Config(path) {}

std::map<std::string, std::string> MimeTypeConf::ReadConfig(){

	string raw_file = Helpers::ReadFile(path);
	vector<string> config = GetConfigLines(raw_file);
	for (vector<string>::const_iterator it=config.begin(); it != config.end(); ++it)
	{
		size_t first_tab = it->find_first_of('\t');
		size_t last_tab = it->find_last_of('\t');

		if (first_tab != string::npos)
		{
			string mime_type = it->substr(0, first_tab);
			StringOperations::Trim(mime_type);
			string file_extensions = it->substr(last_tab + 1);
			vector<string> file_extensions_split = StringOperations::Split(file_extensions, ' ');
			MapFileExtensionsToMimeTypes(file_extensions_split, mime_type);
		}
	}
	return config_items;
}

void MimeTypeConf::MapFileExtensionsToMimeTypes(const vector<string> &file_extensions, const string& mime_type)
{
	for (vector<string>::const_iterator it=file_extensions.begin(); it != file_extensions.end(); ++it)
	{
		string fe = *it;
		StringOperations::Trim(fe);
		config_items[fe] = mime_type;
	}
}

MimeTypeConf::~MimeTypeConf() {
}

