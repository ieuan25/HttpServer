/*
 * DynamicPOSTResource.cpp
 *
 *  Created on: 5 Jan 2014
 *      Author: ieuan
 */

#include "DynamicPOSTResource.h"
#include "StringOperations.h"
using namespace std;

DynamicPOSTResource::DynamicPOSTResource(std::string a, std::string b, std::string c) :
DynamicResource(a,b,c)  {}

void DynamicPOSTResource::SetCGICommandString(char* cmd)
{
	sprintf(cmd, "echo '%s' | /usr/bin/php-cgi", request_data.c_str());
}

void DynamicPOSTResource::SetEnvironment(){

	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("REDIRECT_STATUS", "200", 1);
	setenv("REQUEST_METHOD", "POST", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REMOTE_HOST", "127.0.0.1", 1);
	setenv("HTTP_ACCEPT", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8", 1);
	setenv("SCRIPT_FILENAME", full_path.c_str(), 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	setenv("CONTENT_LENGTH", StringOperations::IntToString(request_data.length()).c_str(), 1);
}

DynamicPOSTResource::~DynamicPOSTResource() {
}
