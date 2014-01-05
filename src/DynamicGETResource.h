/*
 * DynamicGETResource.h
 *
 *  Created on: 5 Jan 2014
 *      Author: ieuan
 */

#ifndef DYNAMICGETRESOURCE_H_
#define DYNAMICGETRESOURCE_H_

#include "DynamicResource.h"

class DynamicGETResource : public DynamicResource {
public:
	DynamicGETResource(std::string, std::string, std::string);
	virtual ~DynamicGETResource();
private:
	void SetEnvironment();
	void SetCGICommandString(char *cmd);
};

#endif /* DYNAMICGETRESOURCE_H_ */
