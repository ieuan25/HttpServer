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
#include <cstring>
#include <string>
#include <vector>
#include <poll.h>

class SockInterface {
public:
	SockInterface(int csock);
	void Read(std::string &buffer, const int max_bytes, int timeout);
	void Write(const std::string &data);
	void Close();
	~SockInterface();
	int socket_fd;

private:
};

#endif /* SOCKINTERFACE_H_ */
