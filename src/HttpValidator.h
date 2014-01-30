/*
 * HttpValidator.h
 *
 *  Created on: 26 Jan 2014
 *      Author: ieuan
 */

#ifndef HTTPVALIDATOR_H_
#define HTTPVALIDATOR_H_

#include <vector>
#include <string>

class HttpValidator {
public:
	HttpValidator(const std::string&);
	void Validate();
	virtual ~HttpValidator();
private:
	const std::string http_raw_request;

};


#endif /* HTTPVALIDATOR_H_ */
