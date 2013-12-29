/*
 * DynamicResource.cpp
 *
 *  Created on: 24 Dec 2013
 *      Author: ieuan
 */

#include "DynamicResource.h"
#include "StringOperations.h"
#include <stdlib.h>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

DynamicResource::DynamicResource(string htdocs, string script_path, string post_data) :
		Resource(htdocs,script_path), post_data(post_data) {}

void DynamicResource::LoadContent(){
	SetEnvironment();
	char cmd[256];
	sprintf(cmd, "echo '%s' | /usr/bin/php-cgi", post_data.c_str());
	FILE* in = popen(cmd, "r");

	char buff[1024];
	bool is_content = false;
	while(fgets(buff, 1024, in) != NULL)
	{
        string line = buff;
		if (is_content) content += buff;
        if (line.length() == 1) is_content = true;
	}
}

void DynamicResource::SetEnvironment(){

	setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
	setenv("REDIRECT_STATUS", "200", 1);
	setenv("REQUEST_METHOD", "POST", 1);
	setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
	setenv("REMOTE_HOST", "127.0.0.1", 1);
	setenv("HTTP_ACCEPT", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8", 1);
	setenv("SCRIPT_FILENAME", full_path.c_str(), 1);
	setenv("CONTENT_TYPE", "application/x-www-form-urlencoded", 1);
	setenv("CONTENT_LENGTH", StringOperations::IntToString(post_data.length()).c_str(), 1);
}

std::string DynamicResource::GetResourceExtension(){
	return "html";
}

DynamicResource::~DynamicResource() {
	// TODO Auto-generated destructor stub
}
