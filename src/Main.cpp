//============================================================================
// Name        : HttpTwo.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/resource.h>
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
#include <fcntl.h>

/*
 * TODO:
 * Clean up Config/MimeType + String operations
 * Use smart pointers to fix double de-allocation
 */

#include "MimeTypes.h"
using namespace std;

#define CONF_PATH "/home/kybu/Code/HttpServer/Http.conf"

int ForkNewProcess();
void Daemonise();
void HandleSigChld();

int main(int argc, char* argv[])
{
	HandleSigChld();

	Config conf(CONF_PATH);
	map<string, string> config = conf.ReadConfig();
	MimeTypeConf mime_type_config(config["mime_path"].c_str());
	map<string, string> mime_type_map = mime_type_config.ReadConfig();

	if (conf.GetConfigItems().at("daemon").compare("yes") == 0)
		Daemonise();

	openlog(NULL, LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Server has started.");
	int client_socket;
	int pid;

	try
	{
		TCPConnection connection(config["port"], config["max_connections"]);
		connection.BindToAddress();
		while(1)
		{
			client_socket = connection.GetClientSocket();
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

void Daemonise()
{
	// code to ensure process can run safely as a daemon
	// allows the daemon to specify it's own filemode creation masks and not inherit limitations of parent shell
	umask(0);

	// Disassociate with the controlling shell/TTY
	if (ForkNewProcess() != 0)
		exit(0);

	// Become process group leaderSS
	setsid();

	// Ignore SIGHUP
	struct sigaction sa;
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		throw runtime_error(strerror(errno));

	// Close file descriptors inherited from parent and set 0,1,2 to null instead of STD_IN/OUT/ERR
	struct rlimit rlim;

	if (getrlimit(RLIMIT_NOFILE, &rlim) < 0)
		throw runtime_error(strerror(errno));

	if (rlim.rlim_max == RLIM_INFINITY)
		rlim.rlim_max = 1024;

	for (unsigned int i=0; i <rlim.rlim_max; ++i)
		close(i);

	int fd0 = open("/dev/null", O_RDWR);
	int fd1 = dup(0);
	int fd2 = dup(0);

	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
		throw runtime_error("File descriptors not correctly set");
}

/* Handler does nothing at the moment. Just demonstrating that a system call
 * can be interrupted.  */
void sigchldHandler(int sigNum)
{
}

void HandleSigChld()
{
	struct sigaction sig_act;
	sig_act.sa_handler = sigchldHandler;
	sigfillset(&sig_act.sa_mask); // Important, blocking all signals while the handler runs.
	sig_act.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT;
	if (sigaction(SIGCHLD, &sig_act, NULL) < 0)
		syslog(LOG_ERR, "Could not register signal handler for SIGCHILD");
}

int ForkNewProcess()
{
	int pid;
	if((pid = fork()) < 0)
		throw runtime_error(strerror(errno));

	return pid;
}
