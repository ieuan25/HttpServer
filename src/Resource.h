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
	Resource(const std::string& htdocs, const std::string& res);
	virtual void LoadContent();
	std::string GetContent() { return content; };
	std::string GetContentLength();
	virtual std::string GetResourceExtension();
	virtual ~Resource();
protected:
	std::string res_path;
	std::string base_url;
	std::string full_path;
	std::string content;

};
#endif /* RESOURCE_H_ */
