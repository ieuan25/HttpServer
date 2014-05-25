/*
 * Exceptions.cpp
 *
 *  Created on: 28 Oct 2013
 *      Author: ieuan
 */

#include "Exceptions.h"

using namespace std;

socket_timeout::socket_timeout(const string& message) : runtime_error(message) {}
file_not_found::file_not_found(const string& message) : runtime_error(message) {}
bad_request::bad_request(const string& message) : runtime_error(message){}

