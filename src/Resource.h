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
	void Read();
	std::string GetContent() { return content; };
	std::string GetContentLength();
	std::string GetResourceExtension();
	virtual ~Resource();
private:
	std::string res_path;
	std::string htdocs_path;
	std::string full_path;
	std::string content;

};
#endif /* RESOURCE_H_ */
