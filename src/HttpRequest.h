/*
 * HttpRequest.h
 *
 *  Created on: 29 Sep 2013
 *      Author: ieuan
 */

#ifndef HTTPREQUEST_H_
#define HTTPREQUEST_H_
#define MAXREQUEST 8001
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include "SockInterface.h"

class HttpRequest {
public:
	HttpRequest();
	virtual ~HttpRequest();
	void Read(SockInterface socketInterface, int timeout);
	std::string GetRequestHeader(std::string);
	std::string GetRequestType() { return request_type; };
	std::string GetRequestedResourcePath() { return requested_resource; };

private:
	std::string raw_request;
	std::string request_type;
	std::string requested_resource;
	std::string http_version;
	std::map<std::string,std::string> headers;
};

#endif
