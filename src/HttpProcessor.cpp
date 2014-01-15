/*
 * RequestProcessor.cpp
 *
 *  Created on: 22 Sep 2013
 *      Author: ieuan
 */

#include "HttpProcessor.h"
#include "HttpRequest.h"
#include "Helpers.h"
#include "Exceptions.h"
#include "StringOperations.h"
#include <syslog.h>

using namespace std;

HttpProcessor::HttpProcessor(int csock, const map<string, string>& conf, const map<string, string>& mtypes) : socket(csock)
{
	htdocs_path = conf.at("htdocs");
	error_pages = conf.at("errors");
	server_name = conf.at("server");
	allow_persistent_connections = conf.at("keep_alive").compare("yes") == 0 ? true : false;
	connection_timeout = StringOperations::StringToInt(conf.at("keep_alive_timeout"));
	mime_map = mtypes;
	httpRequest = NULL;
	httpResponse = NULL;
	resource = NULL;
}

void HttpProcessor::ProcessConnection()
{
	try
	{
		bool keep_alive = true;
		while(keep_alive)
		{
			httpRequest = new HttpRequest();
			httpRequest->Read(socket, connection_timeout);
			keep_alive = KeepConnectionAlive(httpRequest->GetRequestHeader("connection"));

			httpResponse = ProcessRequest();
			httpResponse->Write(socket);

			delete httpResponse;
			delete httpRequest;
		}
	}
	catch(read_timeout &e)
	{
		syslog(LOG_DEBUG, "Persistent connection timed out - terminating.");
	}
	catch(runtime_error &e)
	{
		syslog(LOG_WARNING, "Runtime error thrown. Returning Internal Server Error.");
		ServeErrorPage(500, "/internalerror.html");
	}
	catch(bad_request &e)
	{
		syslog(LOG_DEBUG, "Bad request thrown");
		ServeErrorPage(400, "/badrequest.html");
	}
	catch(file_not_found& e)
	{
		syslog(LOG_DEBUG, "File not found thrown");
		ServeErrorPage(404, "/notfound.html");
	}
	socket.Close();
}

void HttpProcessor::ServeErrorPage(int response_code, const string& error_page)
{
	syslog(LOG_DEBUG, "Serving error page with response code: %d", response_code);

	map<string,string> headers;
	resource = new Resource(error_pages, error_page);
	resource->LoadContent();
	SetResponseHeaders(headers, resource, "close");
	string content = resource->GetContent();

	HttpResponse* response = new HttpResponse(response_code, headers, content);
	response->Write(socket);

	delete resource;
	delete response;
}

HttpResponse* HttpProcessor::ProcessRequest()
{
	syslog(LOG_DEBUG, "Processing request. Request: %s", httpRequest->GetRawRequest().c_str());

	map<string,string> headers;
	resource = GetResource();
	resource->LoadContent();
	SetResponseHeaders(headers, resource, httpRequest->GetRequestHeader("connection"));
	string content = resource->GetContent();

	delete resource;
	return new HttpResponse(200, headers, content);
}

Resource* HttpProcessor::GetResource()
{
	switch(httpRequest->GetResourceType())
	{
		case STATIC:
		{
			return new Resource(htdocs_path, httpRequest->GetRequestedResourcePath());
			break;
		}
		case CGI_POST:
		{
			return new DynamicPOSTResource(htdocs_path, httpRequest->GetRequestedResourcePath(),
					httpRequest->GetPOSTData());
			break;
		}
		case CGI_GET:
		{
			return new DynamicGETResource(htdocs_path, httpRequest->GetRequestedResourcePath(),
					httpRequest->GetGETData());
			break;
		}
		default:
		{
			throw runtime_error("No valid resource type set.");
		}
	}
}

void HttpProcessor::SetResponseHeaders(map<string, string>& headers, Resource* resource, const string& connection)
{
	headers.insert(pair<string, string>("Connection", connection));
	headers.insert(pair<string, string>("Content-Type", mime_map[resource->GetResourceExtension()]));
	headers.insert(pair<string, string>("Date", Helpers::GetCurrentTime()));
	headers.insert(pair<string, string>("Server", server_name));
	headers.insert(pair<string, string>("Content-Length", resource->GetContentLength()));
}

bool HttpProcessor::KeepConnectionAlive(const string& connection_header)
{
	return connection_header.compare("keep-alive")==0 && allow_persistent_connections;
}

HttpProcessor::~HttpProcessor()
{
	delete resource;
	delete httpResponse;
	delete httpRequest;
}

