/*
 * Resource.cpp
 *
 *  Created on: 22 Oct 2013
 *      Author: ieuan
 */

#include "Resource.h"
#include "Helpers.h"
#include <stdexcept>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include "Exceptions.h"
#include "StringOperations.h"
using namespace std;

Resource::Resource(string htdocs, string res) : base_url(htdocs)
{
	res_path = res;

	if (res_path.length() < 2)
	{
		res_path = "/index.html";
	}

	full_path = base_url + res_path;
}

void Resource::LoadContent()
{
	content = Helpers::ReadFile(full_path.c_str());
}

string Resource::GetContentLength()
{
	return StringOperations::IntToString((int)content.length());
}

string Resource::GetResourceExtension(){

	int pos = res_path.find_first_of('.');
	return res_path.substr(pos+1);
}

Resource::~Resource() {}

