//============================================================================
// Name        : HttpTwo.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include "TCPConnection.h"
#include "HttpProcessor.h"
#include "Config.h"
#include <syslog.h>
#include <signal.h>

/*
 * TODO:
 */

#include "MimeTypes.h"
using namespace std;

#define CONF_PATH "/etc/HttpTwo/Http.conf"

typedef int Fork();

int ForkNewProcess();
void HandleSigChld(int);

int main(int argc, char* argv[])
{
	if (signal(SIGCHLD, HandleSigChld) == SIG_ERR)
		syslog(LOG_ERR, "Could not register signal handler for SIGCHILD");

	openlog(NULL, LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Server has started.");
	int client_socket;
	int pid;

	Config conf(CONF_PATH);
	map<string, string> config = conf.ReadConfig();
	MimeTypeConf mime_type_config(config["mime_path"].c_str());
	map<string, string> mime_type_map = mime_type_config.ReadConfig();

	try
	{
		TCPConnection connection(config["port"], config["max_connections"]);
		connection.bindtoaddr();
		while(1)
		{
			client_socket = connection.getclientsock();
			if ((pid = ForkNewProcess()) == 0)
			{
				syslog(LOG_DEBUG, "Http process %d created", getpid());
				HttpProcessor processor(client_socket, config, mime_type_map);
				processor.ProcessConnection();
				exit(0);
			}
			else
			{
				close(client_socket);
			}
		}
	}
	catch(exception& e)
	{
		syslog(LOG_WARNING, "Exiting process due to exception %s", e.what());
		exit(1);
	}

	return 0;
}

void HandleSigChld(int SIG)
{
	if (SIG == SIGCHLD)
	{
		int child_status;
		int child_pid = wait(&child_status);
		syslog(LOG_DEBUG, "Http process %d exiting", child_pid);
	}
}

int ForkNewProcess()
{
	int pid;
	if((pid = fork()) < 0)
		throw runtime_error(strerror(errno));

	return pid;
}
