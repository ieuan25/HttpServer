/*
 * DynamicResource.h
 *
 *  Created on: 24 Dec 2013
 *      Author: ieuan
 */

#ifndef DYNAMICRESOURCE_H_
#define DYNAMICRESOURCE_H_
#include "Resource.h"
#include "HttpRequest.h"
#include <string>

class DynamicResource : public Resource{
public:
	DynamicResource(const std::string&, const std::string&, const std::string&);
	void LoadContent();
	std::string GetResourceExtension();
	virtual ~DynamicResource();

private:
	virtual void SetEnvironment() = 0;
	virtual void SetCGICommandString(char*) = 0;
protected:
	std::string request_data;

};
#endif /* DYNAMICRESOURCE_H_ */
