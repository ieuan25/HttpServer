/*
 * HttpRequest.cpp
 *
 *  Created on: 29 Sep 2013
 *      Author: ieuan
 */

#include "HttpRequest.h"
#include "Helpers.h"
#include <algorithm>
#include <iostream>
#include "StringOperations.h"

using namespace std;

HttpRequest::HttpRequest(){}

void HttpRequest::Read(SockInterface socketInterface, int timeout)
{
	socketInterface.Read(raw_request, MAXREQUEST, timeout);
	vector<string> request_lines = StringOperations::Split(raw_request, '\n');
	vector<string> start_line = StringOperations::Split(request_lines[0], ' ');

	headers = StringOperations::MapStrings(request_lines, ':');

	if (start_line.size() != 3)
		throw std::runtime_error("Invalid HTTP Request: Start Line Incomplete");

	request_type = start_line[0];
	requested_resource = start_line[1];
	http_version = start_line[2];
}

string HttpRequest::GetRequestHeader(string header_name)
{
	map<string, string>::iterator it = headers.find(header_name);

	if (it == headers.end())
		throw std::runtime_error("Could not find requested header");
	else
		return it->second;
}

HttpRequest::~HttpRequest() {
}

