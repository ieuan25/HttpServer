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
#include <sys/select.h>

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

	fd_set read_set;
	FD_ZERO(&read_set);
	FD_SET(socket_fd, &read_set);

	timeval tval;
	tval.tv_sec = timeout;
	tval.tv_usec = 0;

	int select_result = 0;

	if ((select_result = select(socket_fd+1, &read_set, NULL, NULL, &tval)) < 0)
		throw runtime_error("Select error");
	else if (select_result == 0)
		throw socket_timeout("Read timeout.");

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

void SockInterface::Write(const string &response)
{
	unsigned int bytesWritten;
	const char *raw_response = response.data();

	fd_set write_set;
	FD_ZERO(&write_set);
	FD_SET(socket_fd, &write_set);

	timeval tval;
	tval.tv_sec = 5;
	tval.tv_usec = 0;

	int select_result = 0;

	if ((select_result = select(socket_fd+1, NULL, &write_set, NULL, &tval)) < 0)
		throw runtime_error("Select error");
	else if (select_result == 0)
		throw socket_timeout("Write timeout.");

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
