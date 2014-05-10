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
#include "StringOperations.h"
#include "Config.h"
#include <syslog.h>
#include <fcntl.h>
#include "OptionParser.h"
#include "ProcessOperations.h"


#include "MimeTypes.h"
using namespace std;

#define DEFAULT_CONF_PATH "/etc/HttpTwo/Http.conf"
#define VERSION "HttpServer 1.0"


volatile sig_atomic_t children_terminated = 0;

void OverwriteConfigWithOptions(http_options& o, map<string, string>& c);
void SetupChldHandler();
void SetupAlrmHandler();
void sigalarmHandler(int c);

int main(int argc, char* argv[])
{
	try
	{
		int client_socket;
		int pid;
		OptionParser opt_parser(argc, argv);
		cout << "About to read options "<<endl;
		http_options opts;
		if (opt_parser.Parse(opts) < 0)
		{
			cout << "Failure parsing arguments. Exiting." << endl;
			exit(1);
		}

		char conf_path[128];

		if (StringOperations::Isset(opts.conf_path))
			strcpy(conf_path, opts.conf_path);
		else
			strcpy(conf_path, DEFAULT_CONF_PATH);

		Config conf(conf_path);
		map<string, string> config = conf.ReadConfig();
		OverwriteConfigWithOptions(opts, config);
		MimeTypeConf mime_type_config(config["mime_path"].c_str());
		map<string, string> mime_type_map = mime_type_config.ReadConfig();

		const int sock_acc_timeout = StringOperations::StringToInt(config["accept_timeout"]);

		SetupChldHandler();
		SetupAlrmHandler();

		if (config["daemon"].compare("yes") == 0)
		{
			cout << "Daemonising" << endl;
			ProcessOperations::Daemonise();
		}
		openlog(NULL, LOG_PID, LOG_USER);
		syslog(LOG_INFO, "Server has started. Listening on port %s", config["port"].c_str());

		TCPConnection connection(config["port"], config["max_connections"], sock_acc_timeout);
		connection.BindToAddress();

		while(1)
		{
			ProcessOperations::RecoverTerminatedChildren();

			client_socket = connection.GetClientSocket();
			if (client_socket != -1)
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
	}
	catch(exception& e)
	{
		syslog(LOG_WARNING, "Exiting process due to exception %s", e.what());
		exit(1);
	}

	return 0;
}

void OverwriteConfigWithOptions(http_options& opts, map<string,string>& conf){

	if (StringOperations::Isset(opts.daemon))
	{
		conf["daemon"] = opts.daemon;
		cout << "Set daemon in conf to " << conf["daemon"] << endl;
	}
	if (StringOperations::Isset(opts.errors))
		conf["errors"] = opts.errors;
	if (StringOperations::Isset(opts.ht_docs))
		conf["htdocs"] = opts.ht_docs;
	if (StringOperations::Isset(opts.portnum))
		conf["port"] = opts.port_num;
}

/* Signal handlers  */
void sigchldHandler(int sigNum)
{
	SetupChldHandler();
	children_terminated++;
}

void sigalarmHandler(int sigNum)
{
	SetupAlrmHandler();
}

void SetupAlrmHandler()
{
	struct sigaction sig_act;
	sig_act.sa_handler = sigalarmHandler;
	sigfillset(&sig_act.sa_mask);
	memset(&sig_act.sa_flags, '\0', sizeof(sig_act.sa_flags));

	if (sigaction(SIGALRM, &sig_act, NULL) < 0)
		syslog(LOG_ERR, "Could not register signal handler for SIGALARM");
}

void SetupChldHandler()
{
	struct sigaction sig_act;
	sig_act.sa_handler = sigchldHandler;
	sigfillset(&sig_act.sa_mask); // Important, blocking all signals while the handler runs.
	memset(&sig_act.sa_flags, '\0', sizeof(sig_act.sa_flags));
	sig_act.sa_flags = SA_NOCLDSTOP | SA_RESTART;

	if (sigaction(SIGCHLD, &sig_act, NULL) < 0)
		syslog(LOG_ERR, "Could not register signal handler for SIGCHILD");
}
