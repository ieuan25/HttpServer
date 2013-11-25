/*
 * Resource.h
 *
 *  Created on: 22 Oct 2013
 *      Author: ieuan
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_
#include <string>

class Resource {
public:
	Resource(std::string htdocs, std::string res);
	bool isFound() { return found; };
	void Read();
	std::string GetContent() { return content; };
	std::string GetContentLength() { return length; };
	std::string GetResourceExtension();
	virtual ~Resource();
private:
	std::string res_path;
	std::string htdocs_path;
	std::string full_path;
	std::string length;
	std::string content;
	bool found;

};
#endif /* RESOURCE_H_ */
