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

Config::Config(const char * config_path, http_options& opts) : opts(opts){
	strcpy(path, config_path);
}

void Config::SetConfigItem(string k, string v){

	config_items[k] = v;
}

void Config::OverwriteConfigWithOptions(){

	if (StringOperations::Isset(opts.daemon))
		config_items["daemon"] = opts.daemon;
	if (StringOperations::Isset(opts.errors))
		config_items["errors"] = opts.errors;
	if (StringOperations::Isset(opts.ht_docs))
		config_items["htdocs"] = opts.ht_docs;
	if (StringOperations::Isset(opts.portnum))
		config_items["port"] = opts.port_num;
	if (StringOperations::Isset(opts.max_cons))
		config_items["max_connections"] = opts.max_cons;
}

std::map<std::string, std::string> Config::ReadConfig(){

	string raw_file = Helpers::ReadFile(path);
	vector<string> config = GetConfigLines(raw_file);
	config_items = StringOperations::MapStrings(config, '=');
	OverwriteConfigWithOptions();
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
