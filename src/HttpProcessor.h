/*
 * HttpProcessor.h
 *
 *  Created on: 22 Sep 2013
 *      Author: ieuan
 */

#ifndef HTTPPROCESSOR_H_
#define HTTPPROCESSOR_H_
#define MAXREQUEST 8001
#define MAXRESPONSE 200000
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "Resource.h"
#include "DynamicGETResource.h"
#include "DynamicPOSTResource.h"
#include "SockInterface.h"

class HttpProcessor {
public:
	HttpProcessor(int, const std::map<std::string, std::string>&conf, const std::map<std::string, std::string>&mtypes);
	void ProcessConnection();
	virtual ~HttpProcessor();

private:
	std::string htdocs_path;
	std::string error_pages;
	std::string server_name;
	SockInterface socket;
	HttpRequest *httpRequest;
	HttpResponse *httpResponse;
	Resource *resource;
	bool allow_persistent_connections;
	int connection_timeout;
	std::map<std::string, std::string>mime_map;
	void ServeErrorPage(int, const std::string&);
	void SetResponseHeaders(std::map<std::string,std::string>&, Resource*, const std::string& connection);
	bool KeepConnectionAlive(const std::string& connection_header);
	HttpResponse* ProcessRequest();
	Resource* GetResource();
};

#endif /* HTTPPROCESSOR_H_ */
