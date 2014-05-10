/*
 * Config.cpp
 *
 *  Created on: 19 Oct 2013
 *      Author: ieuan
 */

#include "Config.h"
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
#include "Helpers.h"
#include "StringOperations.h"

using namespace std;

Config::Config(const char * config_path) {
	strcpy(path, config_path);
}

void Config::SetConfigItem(string k, string v){

	config_items[k] = v;
}

std::map<std::string, std::string> Config::ReadConfig(){

	string raw_file = Helpers::ReadFile(path);
	vector<string> config = GetConfigLines(raw_file);
	config_items = StringOperations::MapStrings(config, '=');
	return config_items;
}

vector<string> Config::GetConfigLines(const string& raw_config)
{
	vector<string> config_lines = StringOperations::Split(raw_config, '\n');
	config_lines.erase(remove_if(config_lines.begin(), config_lines.end(), Config::CanIgnoreLine), config_lines.end());

	return config_lines;
}

bool Config::CanIgnoreLine(const string& line){

	return line.empty() || line[0] == '#';
}

Config::~Config() {

}
