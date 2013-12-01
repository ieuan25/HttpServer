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
		cout <<"Persistent connection timed out - terminating connection" << endl;
	}
	catch(runtime_error &e)
	{
		ServeErrorPage(500, "/internalerror.html");
	}
	catch(bad_request &e)
	{
		ServeErrorPage(400, "/badrequest.html");
	}
	catch(file_not_found& e)
	{
		ServeErrorPage(404, "/notfound.html");
	}
	socket.Close();
}

void HttpProcessor::ServeErrorPage(int response_code, string error_page)
{
	map<string,string> headers;
	string content;

	Resource error(error_pages, error_page);
	error.Read();
	SetResponseHeaders(headers, error);
	headers.insert(pair<string, string>("Connection", "close"));
	content = error.GetContent();

	HttpResponse* response = new HttpResponse(response_code, headers, content);
	response->Write(socket);
	delete response;
}

HttpResponse* HttpProcessor::ProcessGet(HttpRequest* httpRequest)
{
	int response_code = 200;
	map<string,string> headers;
	string content;

	string resource_path = httpRequest->GetRequestedResourcePath();
	Resource resource(htdocs_path, resource_path);
	resource.Read();
	SetResponseHeaders(headers, resource);
	headers.insert(pair<string, string>("Connection", httpRequest->GetRequestHeader("connection")));
	content = resource.GetContent();

	return new HttpResponse(response_code, headers, content);
}

HttpResponse* HttpProcessor::ProcessPost(HttpRequest* httpRequest)
{
	return NULL;
}

void HttpProcessor::SetResponseHeaders(map<string, string>& headers, Resource& resource)
{
	headers.insert(pair<string, string>("Content-Type", mime_map[resource.GetResourceExtension()]));
	headers.insert(pair<string, string>("Date", Helpers::GetCurrentTime()));
	headers.insert(pair<string, string>("Server", server_name));
	headers.insert(pair<string, string>("Content-Length", resource.GetContentLength()));
}

bool HttpProcessor::KeepConnectionAlive(string connection_header)
{
	return connection_header.compare("keep-alive")==0 && allow_persistent_connections;
}

HttpProcessor::~HttpProcessor()
{
}

