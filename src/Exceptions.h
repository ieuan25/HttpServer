/*
 * Exceptions.h
 *
 *  Created on: 28 Oct 2013
 *      Author: ieuan
 */

#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#include <stdexcept>
#include <string>

class read_timeout: std::runtime_error {
public:
	read_timeout(const std::string& message);
};

class file_not_found: std::runtime_error {
public:
	file_not_found(const std::string& message);
};

class bad_request: std::runtime_error {
public:
	bad_request(const std::string& message);
};

#endif /* EXCEPTIONS_H_ */

