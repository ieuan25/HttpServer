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
		cout << "Runtime error: " << e.what() <<endl;
	}
	socket.Close();
}

bool HttpProcessor::KeepConnectionAlive(string connection_header)
{
	return connection_header.compare("keep-alive")==0 && allow_persistent_connections;
}

HttpResponse* HttpProcessor::ProcessGet(HttpRequest* httpRequest)
{
	string resource_path = httpRequest->GetRequestedResourcePath();
	Resource resource(htdocs_path, resource_path);
	resource.Read();

	int response_code = resource.isFound() ? 200 : 404;

	map<string,string> headers;
	headers.insert(pair<string, string>("Content-Type", mime_map[resource.GetResourceExtension()]));
	headers.insert(pair<string, string>("Date", Helpers::GetCurrentTime()));
	headers.insert(pair<string, string>("Server", server_name));
	headers.insert(pair<string, string>("Content-Length", resource.GetContentLength()));
	headers.insert(pair<string, string>("Connection", httpRequest->GetRequestHeader("connection")));

	string content = resource.GetContent();
	return new HttpResponse(response_code, headers, content);
}

HttpResponse* HttpProcessor::ProcessPost(HttpRequest* httpRequest)
{
	return NULL;
}

HttpProcessor::~HttpProcessor()
{
}

