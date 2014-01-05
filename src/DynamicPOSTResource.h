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
	DynamicPOSTResource(std::string, std::string, std::string);
	virtual ~DynamicPOSTResource();
private:
	void SetEnvironment();
	void SetCGICommandString(char*);
};

#endif /* DYNAMICPOSTRESOURCE_H_ */
