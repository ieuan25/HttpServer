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

/*
 * TODO:
 * Clean up Config/MimeType + String operations
 * Use smart pointers to fix double de-allocation
 */

#include "MimeTypes.h"
using namespace std;

#define DEFAULT_CONF_PATH "/etc/HttpTwo/Http.conf"
#define VERSION "HttpServer 1.0"


typedef struct {
	int version;
	int help;
	char conf_path[64];
} options;

volatile sig_atomic_t children_terminated = 0;

int ForkNewProcess();
void Daemonise();
void SetupChldHandler();
void RecoverTerminatedChildren();
int ParseOpts(int argc, char* argv[], options& options);
void SetupAlrmHandler();
void sigalarmHandler(int c);

int main(int argc, char* argv[])
{
	options http_opts;
	strcpy(http_opts.conf_path, DEFAULT_CONF_PATH);
	http_opts.version = 0;
	http_opts.help = 0;

	if (ParseOpts(argc, argv, http_opts) == -1)
		exit(0);

	if (http_opts.help == 1)
	{
		cout << "HttpServer help" << endl;
		cout << "-v	prints version" <<endl;
		cout << "-c	specify the path to the config file" <<endl;
		exit(0);
	}
	if (http_opts.version == 1)
	{
		cout << VERSION << endl;
		exit(0);
	}

	try
	{
		int client_socket;
		int pid;

		Config conf(http_opts.conf_path);
		map<string, string> config = conf.ReadConfig();
		MimeTypeConf mime_type_config(config["mime_path"].c_str());
		map<string, string> mime_type_map = mime_type_config.ReadConfig();
		const int sock_acc_timeout = StringOperations::StringToInt(config["accept_timeout"]);

		SetupChldHandler();
		SetupAlrmHandler();

		if (conf.GetConfigItems().at("daemon").compare("yes") == 0)
			Daemonise();

		openlog(NULL, LOG_PID, LOG_USER);
		syslog(LOG_INFO, "Server has started. Listening on port %s", config["port"].c_str());

		TCPConnection connection(config["port"], config["max_connections"]);
		connection.BindToAddress();

		while(1)
		{
			alarm(sock_acc_timeout);
			if (children_terminated)
				RecoverTerminatedChildren();

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
			alarm(0);
		}
	}
	catch(exception& e)
	{
		syslog(LOG_WARNING, "Exiting process due to exception %s", e.what());
		exit(1);
	}

	return 0;
}

int ParseOpts(int argc, char* argv[], options& options)
{
	int ret;
	opterr = 0;

	while ((ret = getopt(argc, argv, "vhc:")) != -1)
	{
		switch(ret)
		{
			case 'v':
				options.version = 1;
				break;
			case 'h':
				options.help = 1;
				break;
			case 'c':
				strcpy(options.conf_path,optarg);
				break;
			case '?':
				cerr << "Option " << (char) optopt << " requires an argument" << endl;
				return -1;
			default:
				break;
		}
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

void RecoverTerminatedChildren()
{
	int pid;
	int status;

	while (children_terminated > 0)
	{
		if ((pid = wait(&status)) < 0)
			throw runtime_error(strerror(errno));
		children_terminated--;
		syslog(LOG_WARNING, "Child process with pid %d terminated with status %d", pid, status);
	}

}

void SetupAlrmHandler()
{
	struct sigaction sig_act;
	sig_act.sa_handler = sigalarmHandler;
	sigfillset(&sig_act.sa_mask);

	// IMPORTANT!!! sig_act field values are undefined until they are explicitly
	// initialised. In other words they might contain rubbish.
	// This applies to any struct, variable, etc in C/C++ if no value is assigned to it.
	// In my case not setting sa_flags caused alarm signal not being delivered.
	// I am guessing that its rubbish value actually set SA_RESTART.
	//
	// After I added this line it started working as expected.
	sig_act.sa_flags = 0;

	if (sigaction(SIGALRM, &sig_act, NULL) < 0)
		syslog(LOG_ERR, "Could not register signal handler for SIGALARM");
}

void SetupChldHandler()
{
	struct sigaction sig_act;
	sig_act.sa_handler = sigchldHandler;
	sigfillset(&sig_act.sa_mask); // Important, blocking all signals while the handler runs.
	sig_act.sa_flags = SA_NOCLDSTOP | SA_RESTART;
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
