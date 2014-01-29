/*
 * SockInterface.cpp
 *
 *  Created on: 22 Oct 2013
 *      Author: ieuan
 */

#include "SockInterface.h"
#include <iostream>
#include "Exceptions.h"
#include "Helpers.h"
#include "StringOperations.h"

using namespace std;

SockInterface::SockInterface(int csock) : socket_fd(csock) {

}

void SockInterface::Close()
{
	if (close(socket_fd) < 0)
		throw runtime_error("Error closing socket");
}

void SockInterface::Read(string &request, const int max_bytes, int timeout)
{
	int bytesRead;
	char buff[max_bytes];

	PollSocket(timeout);

	if ((bytesRead = read(socket_fd, buff, max_bytes)) > 0)
	{
		if(bytesRead == max_bytes)
		{
			string max = StringOperations::IntToString(max_bytes);
			throw runtime_error("Request too large. A maximum request of " + max + " is allowed.");
		}
	}
	else if (bytesRead == 0)
	{
		throw runtime_error(strerror(errno));
	}
	else
	{
		throw runtime_error("End of file found");
	}
	request = buff;
}

void SockInterface::PollSocket(int timeout)
{
	struct pollfd readpoll;
	int pollResult = 0;

	readpoll.fd = socket_fd;
	readpoll.events = POLLIN;

	if ((pollResult = poll(&readpoll, 1, timeout*1000)) < 0)
	{
		throw runtime_error("Error polling socket");
	}
	else if (pollResult == 0)
	{
		throw read_timeout("timeout");
	}
}

void SockInterface::Write(const string &response)
{
	unsigned int bytesWritten;
	const char *raw_response = response.data();

	if ((bytesWritten = write(socket_fd, raw_response, response.length())) > 0)
	{
		if (bytesWritten != response.length())
			throw runtime_error("Could not write whole file");
	}
	else
	{
		throw runtime_error(strerror(errno));
	}
}

SockInterface::~SockInterface() {
}
