/*
 * SockInterface.h
 *
 *  Created on: 22 Oct 2013
 *      Author: ieuan
 */

#ifndef SOCKINTERFACE_H_
#define SOCKINTERFACE_H_

#include <stdio.h>
#include <unistd.h>
#include <stdexcept>
#include <errno.h>
#include <stdlib.h>
#include <stdlib.h>
#include <cstring>
#include <string>
#include <poll.h>

class SockInterface {
public:
	SockInterface(int csock);
	void Read(std::string &buffer, const int max_bytes, int timeout) const;
	void Write(const std::string &data) const;
	void Close();
	void PollSocket(int) const;
	virtual ~SockInterface();
	int socket_fd;

private:
};

#endif /* SOCKINTERFACE_H_ */
