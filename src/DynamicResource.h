/*
 * DynamicResource.h
 *
 *  Created on: 24 Dec 2013
 *      Author: ieuan
 */

#ifndef DYNAMICRESOURCE_H_
#define DYNAMICRESOURCE_H_
#include "Resource.h"
#include <string>

class DynamicResource : public Resource{
public:
	DynamicResource(std::string, std::string, std::string);
	void LoadContent();
	std::string GetResourceExtension();
	virtual ~DynamicResource();

private:
	void SetEnvironment();
	std::string post_data;

};
#endif /* DYNAMICRESOURCE_H_ */
