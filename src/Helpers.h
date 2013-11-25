/*
 * HttpHelpers.h
 *
 *  Created on: 29 Sep 2013
 *      Author: ieuan
 */

#ifndef HTTPHELPERS_H_
#define HTTPHELPERS_H_
#include <vector>
#include <string>
#include <sstream>
#include <map>

class Helpers {
public:
	Helpers();
	virtual ~Helpers();
	static std::string ReadFile(const char *filename);
	static std::string GetCurrentTime();
private:
    static void CopyBufferToString(std::string&, char*, int);

};

#endif /* HTTPHELPERS_H_ */
