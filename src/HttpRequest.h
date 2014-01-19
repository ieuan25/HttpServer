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

enum ResourceType { STATIC, CGI_POST, CGI_GET };

class HttpRequest {
public:
	HttpRequest();
	~HttpRequest();
	void Read(SockInterface& socketInterface, int timeout);
	std::string GetRequestHeader(const std::string&);
	std::string GetRequestedResourcePath() { return requested_resource; }
	std::string GetRawRequest() { return raw_request; };
	std::string GetPOSTData() { return post_data; };
	std::string GetGETData() { return get_data; };
	ResourceType GetResourceType() { return resource_type; };

private:
	void Validate();
	void SetRequestValues();
	std::vector<std::string> status_line;
	std::vector<std::string> request_lines;
	std::string raw_request;
	std::string requested_resource;
	std::string http_version;
	std::string post_data;
	std::string get_data;
	ResourceType resource_type;
	std::map<std::string,std::string> headers;
};

#endif
