/*
 * DynamicResource.cpp
 *
 *  Created on: 24 Dec 2013
 *      Author: ieuan
 */

#include "DynamicResource.h"
#include "StringOperations.h"
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

DynamicResource::DynamicResource(const string& htdocs, const string& script_path, const string& data) :
		Resource(htdocs,script_path), request_data(data){}

void DynamicResource::LoadContent()
{
	SetEnvironment();
	char cmd[256];
	SetCGICommandString(cmd);

	FILE* in = popen(cmd, "r");

	char buff[1024];
	cout << cmd << endl;
	while(fgets(buff, 1024, in) != NULL)
	{
		content += buff;
	}
}

DynamicResource::~DynamicResource() {
}
