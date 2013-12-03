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
#include "MimeTypes.h"
#include <syslog.h>

/*
 * TODO:
 * Read up on processes and how to configure to use PHP
 * Read up on signals to find a way to spawn process nicely
 */

using namespace std;

#define CONF_PATH "/etc/HttpTwo/Http.conf"

int ForkNewProcess();

int main(int argc, char* argv[])
{
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


// calls fork twice so that new process is an orphan and init process will take care of cleanup
int ForkNewProcess()
{
	int pid;
	if((pid = fork()) < 0)
		throw runtime_error(strerror(errno));

	if(pid == 0)
	{
		if((pid = fork()) < 0)
			throw runtime_error(strerror(errno));

		if (pid > 0)
			exit(0);
		else
		{
			// ensure that parent has exited before returning
			while (getppid() > 1)
				sleep(0.1);
		}
	}
	else
	{
		int status;
		if (waitpid(pid, &status, 0) != pid)
			throw runtime_error(strerror(errno));
	}
	return pid;
}
