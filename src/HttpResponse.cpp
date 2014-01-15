/*
 * HttpResponse.cpp
 *
 *  Created on: 29 Sep 2013
 *      Author: ieuan
 */

#include "HttpResponse.h"
#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include "Helpers.h"
#include "StringOperations.h"

using namespace std;

HttpResponse::HttpResponse(int response_code, const map<string,string>& headers, const string& content) : response_code(response_code),
		http_version("HTTP/1.1")
{
	_headers = headers;
	_body = content;
	SetStatus();
}

void HttpResponse::SetStatus()
{
	string response_message;

	switch(response_code)
	{
		case 200:
			response_message = "Ok";
			break;

		case 404:
			response_message = "Not Found";
			break;

		case 500:
			response_message = "Internal Server Error";
			break;
	}

	status_line = http_version + " " + StringOperations::IntToString(response_code) + " " + response_message;
}

void HttpResponse::Write(const SockInterface& socketInterface)
{
	string raw_headers;
	for (map<string, string>::iterator it=_headers.begin(); it!=_headers.end(); ++it)
	{
		raw_headers += it->first + ": " + it->second + "\r\n";
	}
	string response = status_line + "\r\n" + raw_headers + "\r\n" + _body;

	socketInterface.Write(response);
}


HttpResponse::~HttpResponse()
{}

