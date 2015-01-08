/*
 * ProcessOperations.cpp
 *
 *  Created on: 5 May 2014
 *      Author: ieuan
 */

#include "ProcessOperations.h"
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <errno.h>
#include <fcntl.h>
#include <syslog.h>

using namespace std;

ProcessOperations::ProcessOperations() {
}

void ProcessOperations::RecoverTerminatedChildren(int readFd)
{
	int pid;
	int status;

	while (children_terminated > 0)
	{
		if ((pid = wait(&status)) < 0)
			throw runtime_error(strerror(errno));

		// Reading a child termination trigger from the pipe so that
		// the main loop does not spin on select(...)
		char buf;
		read(readFd, &buf, 1);

		children_terminated--;
		syslog(LOG_WARNING, "Child process with pid %d terminated with status %d. %d child processes left.", pid, status, children_terminated);
	}
}

int ProcessOperations::ForkNewProcess(){

	int pid;
	if((pid = fork()) < 0)
		throw runtime_error(strerror(errno));

	return pid;
}

void ProcessOperations::Daemonise() {

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

ProcessOperations::~ProcessOperations() {
	// TODO Auto-generated destructor stub
}

