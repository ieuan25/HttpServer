/*
 * StringOperations.h
 *
 *  Created on: 3 Nov 2013
 *      Author: ieuan
 */

#ifndef STRINGOPERATIONS_H_
#define STRINGOPERATIONS_H_
#include <vector>
#include <string>
#include <sstream>
#include <map>


class StringOperations {
public:
	StringOperations();
	static std::string Trim(std::string&);
	static int StringToInt(std::string);
	static std::string IntToString(int);
	static std::vector<std::string> Split(const std::string &s, char delim);
    static std::map<std::string, std::string> MapStrings(const std::vector<std::string>& elements, const char seperator);
	virtual ~StringOperations();
};
#endif /* STRINGOPERATIONS_H_ */
