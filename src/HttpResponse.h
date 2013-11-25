/*
 * HttpResponse.h
 *
 *  Created on: 29 Sep 2013
 *      Author: ieuan
 */

#ifndef HTTPRESPONSE_H_
#define HTTPRESPONSE_H_
#include <string>
#include <map>
#include <stdexcept>
#include "SockInterface.h"

class HttpResponse {
public:
	HttpResponse(int, std::map<std::string,std::string>, std::string);
	virtual ~HttpResponse();
	void SetStatus();
	bool AddHeader(std::string, std::string);
	void SetBody(std::string s) { body = s; } ;
	void Write(SockInterface);

private:
	std::map<std::string, std::string> response_headers;
	std::string body;
	int response_code;
	std::string status_line;
	std::map<std::string,std::string> _headers;
	std::string http_version;
};

#endif /* HTTPRESPONSE_H_ */
