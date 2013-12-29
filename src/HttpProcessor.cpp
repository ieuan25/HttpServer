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

HttpProcessor::HttpProcessor(int csock, map<string, string>& conf, const map<string, string>& mtypes) : socket(csock)
{
	htdocs_path = conf["htdocs"];
	error_pages = conf["errors"];
	server_name = conf["server"];
	allow_persistent_connections = conf["keep_alive"].compare("yes") == 0 ? true : false;
	connection_timeout = StringOperations::StringToInt(conf["keep_alive_timeout"]);
	mime_map = mtypes;
}

void HttpProcessor::ProcessConnection()
{
	try
	{
		bool keep_alive = true;
		while(keep_alive)
		{
			HttpRequest *httpRequest = new HttpRequest();
			httpRequest->Read(socket, connection_timeout);
			keep_alive = KeepConnectionAlive(httpRequest->GetRequestHeader("connection"));

			HttpResponse *httpResponse;

			if (httpRequest->GetRequestType().compare("GET") == 0)
			{
				httpResponse = ProcessGet(httpRequest);
			}
			else if(httpRequest->GetRequestType().compare("POST") == 0)
			{
				httpResponse = ProcessPost(httpRequest);
			}

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

void HttpProcessor::ServeErrorPage(int response_code, string error_page)
{
	map<string,string> headers;
	string content;

	Resource *static_resource = new Resource(error_pages, error_page);
	static_resource->LoadContent();
	SetResponseHeaders(headers, static_resource);
	headers.insert(pair<string, string>("Connection", "close"));
	content = static_resource->GetContent();
	syslog(LOG_DEBUG, "Serving error page with response code: %d", response_code);
	HttpResponse* response = new HttpResponse(response_code, headers, content);
	response->Write(socket);
	delete static_resource;
	delete response;
}

HttpResponse* HttpProcessor::ProcessGet(HttpRequest* httpRequest)
{
	syslog(LOG_DEBUG, "Processing %s request. Request: %s",
			httpRequest->GetRequestType().c_str(), httpRequest->GetRawRequest().c_str());

	int response_code = 200;
	map<string,string> headers;

	Resource *static_resource = new Resource(htdocs_path, httpRequest->GetRequestedResourcePath());
	static_resource->LoadContent();
	SetResponseHeaders(headers, static_resource);
	headers.insert(pair<string, string>("Connection", httpRequest->GetRequestHeader("connection")));
	string content = static_resource->GetContent();
	delete static_resource;
	return new HttpResponse(response_code, headers, content);
}

HttpResponse* HttpProcessor::ProcessPost(HttpRequest* httpRequest)
{
	syslog(LOG_DEBUG, "Processing POST request. Request: %s",
			httpRequest->GetRawRequest().c_str());

	int response_code = 200;
	map<string,string> headers;

	DynamicResource* cgi_resource = new DynamicResource(htdocs_path, httpRequest->GetRequestedResourcePath(), httpRequest->GetPostData());
	cgi_resource->LoadContent();
	SetResponseHeaders(headers, cgi_resource);
	headers.insert(pair<string, string>("Connection", httpRequest->GetRequestHeader("connection")));
	cout << "MimeType1 " << cgi_resource->GetResourceExtension() << endl;
	string content = cgi_resource->GetContent();
	delete cgi_resource;
	return new HttpResponse(response_code, headers, content);
}

void HttpProcessor::SetResponseHeaders(map<string, string>& headers, Resource* resource)
{
	cout << "MimeType " << resource->GetResourceExtension() << endl;
	headers.insert(pair<string, string>("Content-Type", mime_map[resource->GetResourceExtension()]));
	headers.insert(pair<string, string>("Date", Helpers::GetCurrentTime()));
	headers.insert(pair<string, string>("Server", server_name));
	headers.insert(pair<string, string>("Content-Length", resource->GetContentLength()));
}

bool HttpProcessor::KeepConnectionAlive(string connection_header)
{
	return connection_header.compare("keep-alive")==0 && allow_persistent_connections;
}

HttpProcessor::~HttpProcessor()
{
}

