//============================================================================
// Name        : HttpTwo.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/*
 * TODO:
 * Refactor and re-familiarise
 * Clean up the CGI stuff - must be a better way
 * Add a multi-threaded option - as opposed to using processes
 * Try and use open-ssl to support https
 */

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <netdb.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <sys/wait.h>
#include "TCPConnection.h"
#include "HttpProcessor.h"
#include "StringOperations.h"
#include "Config.h"
#include <syslog.h>
#include <fcntl.h>
#include "OptionParser.h"
#include "ProcessOperations.h"

#include <boost/date_time.hpp>
#include <thread>
#include <chrono>


#include "MimeTypes.h"
using namespace std;

#ifndef DEFAULT_CONF_PATH
# define DEFAULT_CONF_PATH "/etc/HttpTwo/Http.conf"
#endif
#define VERSION "HttpServer 1.0"

volatile sig_atomic_t children_terminated = 0;
void InitialiseServer(map<string, string>& config);
void SetupSigChildHandler();
void PrintHelpAndExit();
void PrintVersionAndExit();
void HandleSignals(int);

int fd[2]; // my pipe file descriptors, global so can pass to the signal handler

int main(int argc, char* argv[])
{
	try
	{
		if (pipe(fd) < 0)
		{
			syslog(LOG_DEBUG, "Pipe error");
		}

        // Handle signals here
		thread myThread(HandleSignals, fd[1]);

        // Block all signals here
        sigset_t block_set;
        sigfillset(&block_set);
        sigprocmask(SIG_BLOCK, &block_set, NULL);

		int client_socket;
		int pid;
		char conf_path[128];
		OptionParser opt_parser(argc, argv);
		http_options opts;



		if (opt_parser.Parse(opts) < 0)
			exit(1);

		if (opts.help == 1)
			PrintHelpAndExit();

		if (opts.version == 1)
			PrintVersionAndExit();

		if (StringOperations::Isset(opts.conf_path))
			strcpy(conf_path, opts.conf_path);
		else
			strcpy(conf_path, DEFAULT_CONF_PATH);

		Config conf(conf_path, opts);
		map<string, string> config = conf.ReadConfig();

		MimeTypeConf mime_type_config(config["mime_path"].c_str(), opts);
		map<string, string> mime_type_map = mime_type_config.ReadConfig();

		InitialiseServer(config);

		TCPConnection connection(config["port"], config["max_connections"], StringOperations::StringToInt(config["accept_timeout"]));
		connection.BindToAddress();

        while(1)
		{
			ProcessOperations::RecoverTerminatedChildren();

			client_socket = connection.AcceptClient(fd[0]);
			if (client_socket > -1)
			{
				if ((pid = ProcessOperations::ForkNewProcess()) == 0)
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
		myThread.join();
	}
	catch(exception& e)
	{
		syslog(LOG_WARNING, "Exiting process due to exception %s", e.what());
		exit(1);
	}

	return 0;
}

void InitialiseServer(map<string, string>& config){

	if (config["daemon"].compare("yes") == 0)
		ProcessOperations::Daemonise();

	//SetupSigChildHandler();

	openlog(NULL, LOG_PID, LOG_USER);
	syslog(LOG_INFO, "Server has started. Listening on port %s", config["port"].c_str());
}

void PrintHelpAndExit(){

	cout << "Usage HttpServer [OPTIONS]"<<endl;
	cout << "GNU long option"<<"\t\t\t\t"<<"Meaning"<<endl;
	cout << "--version"<<"\t\t\t\t"<<"prints version info"<<endl;
	cout << "--daemon[=yes/no]"<<"\t\t\t"<<"allows server to run as a daemon"<<endl;
	cout << "--confpath[=/etc/HttpConf.conf]"<<"\t\t"<<"sets the config file location"<<endl;
	cout << "--portnum[=80]"<<"\t\t\t\t"<<"sets the port number that the server listens for connections on"<<endl;
	cout << "--maxcons[=10]"<<"\t\t\t\t"<<"sets the maximum number of client connections to allow at any one time"<<endl;
	cout << "--htdocs[=/var/htdocs]"<<"\t\t\t"<<"sets the htdocs directory location"<<endl;
	cout << "--errors[=/var/errors]"<<"\t\t\t"<<"sets the error pages directory location"<<endl;
	exit(0);
}

void PrintVersionAndExit(){

	cout << VERSION << endl;
	exit(0);
}

/* Signal handlers  */
void sigchldHandler(int sigNum)
{
	SetupSigChildHandler();
	children_terminated++;
}

void HandleSignals(int writeFd)
{
	SetupSigChildHandler();
	syslog(LOG_ERR, "Doing some signal handling!!");
	while(true)
	{
		this_thread::sleep_for(chrono::seconds {5});

		if (children_terminated) {
			syslog(LOG_INFO, "Sending a trigger into the pipe that a child terminated.");

			write(writeFd, "T", 1);
		}
	}

}

void SetupSigChildHandler()
{
	struct sigaction sig_act;
	sig_act.sa_handler = sigchldHandler;
	sigfillset(&sig_act.sa_mask); // Important, blocking all signals while the handler runs.
	memset(&sig_act.sa_flags, '\0', sizeof(sig_act.sa_flags));
	sig_act.sa_flags = SA_NOCLDSTOP | SA_RESTART;

	if (sigaction(SIGCHLD, &sig_act, NULL) < 0)
		syslog(LOG_ERR, "Could not register signal handler for SIGCHILD");
}
