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
	HttpResponse(int, const std::map<std::string,std::string>&, const std::string&);
	~HttpResponse();
	void SetStatus();
	bool AddHeader(std::string, std::string);
	void SetBody(std::string s) { _body = s; } ;
	void Write(SockInterface&);

private:
	std::map<std::string, std::string> response_headers;
	std::string _body;
	int response_code;
	std::string status_line;
	std::map<std::string,std::string> _headers;
	std::string http_version;
};

#endif /* HTTPRESPONSE_H_ */
