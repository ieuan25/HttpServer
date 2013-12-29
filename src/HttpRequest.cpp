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
#include "Exceptions.h"

using namespace std;

HttpRequest::HttpRequest(){}

void HttpRequest::Read(SockInterface socketInterface, int timeout)
{
	socketInterface.Read(raw_request, MAXREQUEST, timeout);
	request_lines = StringOperations::Split(raw_request, '\n');
	status_line = StringOperations::Split(request_lines[0], ' ');
	Validate();
	SetRequestValues();
}

void HttpRequest::SetRequestValues()
{
	headers = StringOperations::MapStrings(request_lines, ':');
	request_type = status_line[0];
	requested_resource = status_line[1];
	http_version = status_line[2];
	post_data = request_lines[request_lines.size()-1];
}

void HttpRequest::Validate()
{
	if (status_line.size() != 3)
		throw std::runtime_error("Invalid HTTP Request: Start Line Incomplete");

	if (status_line[0].compare("POST") != 0 && status_line[0].compare("GET") != 0)
		throw std::runtime_error("Invalid request type");
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

