/*
 * DynamicPOSTResource.h
 *
 *  Created on: 5 Jan 2014
 *      Author: ieuan
 */

#ifndef DYNAMICPOSTRESOURCE_H_
#define DYNAMICPOSTRESOURCE_H_
#include "DynamicResource.h"
#include <string>

class DynamicPOSTResource : public DynamicResource {
public:
	DynamicPOSTResource(const std::string&, const std::string&, const std::string&);
	~DynamicPOSTResource();
private:
	void SetEnvironment();
	void SetCGICommandString(char*);
};

#endif /* DYNAMICPOSTRESOURCE_H_ */
