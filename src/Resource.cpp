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
#include "Exceptions.h"
#include "StringOperations.h"
using namespace std;

Resource::Resource(string htdocs, string res) : htdocs_path(htdocs), found(false)
{
	res_path = res;

	if (res_path.length() < 2)
	{
		res_path = "/index.html";
	}

	full_path = htdocs_path + res_path;
}

void Resource::Read()
{
	try
	{
		content = Helpers::ReadFile(full_path.c_str());
		length = StringOperations::IntToString((int)content.length());
		found = true;
	}
	catch(file_not_found& e)
	{
		content = "<!DOCTYPE HTML PUBLIC <html>"
				"<head><title>404 Not Found</title></head>"
				"<body>"
				"<h1>Not Found</h1>"
				"<p>The requested URL was not found on this server.</p>"
				"<hr>"
				"</body></html>";

		length = StringOperations::IntToString((int)content.length());
	}
}

string Resource::GetResourceExtension(){

	int pos = res_path.find_first_of('.');
	return res_path.substr(pos+1);
}

Resource::~Resource() {}

