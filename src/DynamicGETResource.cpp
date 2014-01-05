/*
 * DynamicGETResource.cpp
 *
 *  Created on: 5 Jan 2014
 *      Author: ieuan
 */

#include "DynamicGETResource.h"
#include "StringOperations.h"
#include <iostream>

using namespace std;
DynamicGETResource::DynamicGETResource(std::string a, std::string b, std::string c) :
DynamicResource(a,b,c)  {}

void DynamicGETResource::SetCGICommandString(char *cmd) {
	sprintf(cmd, "/usr/bin/php-cgi");
}

void DynamicGETResource::SetEnvironment(){

	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("REDIRECT_STATUS", "200", 1);
	setenv("REQUEST_METHOD", "GET", 1);
	setenv("QUERY_STRING", request_data.c_str(), 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REMOTE_HOST", "127.0.0.1", 1);
	setenv("HTTP_ACCEPT", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8", 1);
	setenv("SCRIPT_FILENAME", full_path.c_str(), 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
}

DynamicGETResource::~DynamicGETResource() {
}
