/*
 * HttpValidator.cpp
 *
 *  Created on: 26 Jan 2014
 *      Author: ieuan
 */

#include "HttpValidator.h"
#include "Exceptions.h"
#include "StringOperations.h"
#include <boost/regex.hpp>

using namespace std;
using namespace boost;

HttpValidator::HttpValidator(const std::string& request) : http_raw_request(request) {
}

void HttpValidator::Validate()
{
	cout << http_raw_request << endl;
	string sl_pattern = "^(POST|GET)\\s/.*\\sHTTP/1.1\\s*\n(.*:\\s.*\n).*$";
	regex http_valid_regex(sl_pattern);
	cmatch sl_matches;

	if (!regex_match(http_raw_request.c_str(), sl_matches, http_valid_regex))
		throw bad_request("Request is not valid HTTP.");

}

HttpValidator::~HttpValidator() {
}

