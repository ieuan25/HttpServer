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
	request_validator = NULL;
}

void HttpRequest::Read(SockInterface& socketInterface, int timeout)
{
	string raw_request;
	socketInterface.Read(raw_request, MAXREQUEST, timeout);
	request_validator = new HttpValidator(raw_request);
	request_validator->Validate();

	request_lines = StringOperations::Split(raw_request, '\n');
	http_status_elements = StringOperations::Split(request_lines[0], ' ');
	http_version = http_status_elements[2];
	resource_path = FindRequestedResourcePath(http_status_elements[1]);
	resource_type = FindRequestedResourceType(http_status_elements[0], http_status_elements[1]);
	request_data = FindRequestParametersIfNotStatic(resource_type, http_status_elements[1]);
	headers = StringOperations::MapStrings(request_lines, ':');
}

string HttpRequest::FindRequestParametersIfNotStatic(ResourceType res_type, const string& res_path)
{
	if (res_type == CGI_GET)
	{
		if (StringOperations::Contains(res_path, "?"))
			return StringOperations::Split(res_path, '?')[1];
		else
			throw runtime_error("CGI Get request must contain parameters");
	}
	if (res_type == CGI_POST)
		return request_lines[request_lines.size()-1];
	else
		return "";
}

string HttpRequest::FindRequestedResourcePath(const string& resource_string)
{
	size_t pos = resource_string.find('?');

	if (pos == string::npos)
		return resource_string;
	else
		return resource_string.substr(0, pos);
}

ResourceType HttpRequest::FindRequestedResourceType(const string& http_method, const string& resource_string)
{
	if (StringOperations::Contains(resource_string, "cgi-bin"))
	{
		if (http_method.compare("POST") == 0)
			return CGI_POST;
		else
			return CGI_GET;
	}
	else
		return STATIC;
}

string HttpRequest::GetRequestHeader(const string& header_name)
{
	map<string, string>::iterator it = headers.find(header_name);

	if (it == headers.end())
		throw std::runtime_error("Could not find requested header");
	else
		return it->second;
}

HttpRequest::~HttpRequest() {
}

