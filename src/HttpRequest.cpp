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

HttpRequest::HttpRequest(){
	resource_type = STATIC;
}

void HttpRequest::Read(SockInterface socketInterface, int timeout)
{
	socketInterface.Read(raw_request, MAXREQUEST, timeout);
	request_lines = StringOperations::Split(raw_request, '\n');
	status_line = StringOperations::Split(request_lines[0], ' ');
	SetRequestValues();
	Validate();
}

void HttpRequest::SetRequestValues()
{
	headers = StringOperations::MapStrings(request_lines, ':');
	requested_resource = status_line[1];
	http_version = status_line[2];

	if (StringOperations::Contains(requested_resource, "cgi-bin"))
	{
		if (status_line[0].compare("POST") == 0)
			resource_type = CGI_POST;
		else
			resource_type = CGI_GET;
	}

	if (resource_type == CGI_GET)
	{
		if (StringOperations::Contains(requested_resource, "?"))
			get_data = StringOperations::Split(requested_resource, '?')[1];
		else
			throw bad_request("CGI Get request must contain parameters");
	}
	if (resource_type == CGI_POST)
	{
		post_data = request_lines[request_lines.size()-1];
	}

	size_t pos;
	if ((pos = requested_resource.find('?')) != string::npos)
	{
		requested_resource = requested_resource.substr(0, pos);
	}
}

void HttpRequest::Validate()
{
	if (status_line.size() != 3)
		throw bad_request("Invalid HTTP Request: Start Line Incomplete");

	if (status_line[0].compare("POST") != 0 && status_line[0].compare("GET") != 0)
		throw bad_request("Invalid request type");
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

