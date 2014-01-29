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
#include "HttpValidator.h"

enum ResourceType { STATIC, CGI_POST, CGI_GET };

class HttpRequest {
public:
	HttpRequest();
	~HttpRequest();
	void Read(SockInterface& socketInterface, int timeout);
	std::string GetRequestHeader(const std::string&);
	std::string GetRequestedResourcePath() { return resource_path; }
	std::string GetRequestData() { return request_data; };
	ResourceType GetResourceType() { return resource_type; };

private:
	std::string FindRequestedResourcePath(const std::string&);
	void SetRequestValues();
	std::string GetRequestedResource();
	std::string FindRequestParametersIfNotStatic(ResourceType res_type, const std::string& res_path);
	ResourceType FindRequestedResourceType(const std::string& http_method, const std::string& resource_string);
	std::vector<std::string> http_status_elements;
	std::vector<std::string> request_lines;
	std::string resource_path;
	std::string http_version;
	std::string request_data;
	ResourceType resource_type;
	HttpValidator *request_validator;
	std::map<std::string,std::string> headers;
};

#endif
