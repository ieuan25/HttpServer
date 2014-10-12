/*
 * TCPConnection.h
 *
 *  Created on: 22 Sep 2013
 *      Author: ieuan
 */

#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdexcept>
#include <string>

typedef struct addrinfo addinfo;

class TCPConnection {
public:
	TCPConnection(const std::string& port, const std::string& max_cons, int sock_timeout);
	~TCPConnection();
	int BindToAddress();
	int AcceptClient(int);
	std::string port;
	int sock_timeout;

private:
	int err;
	int sockfd;
	addinfo *addresstobind;
	int max_connections;
};

#endif /* TCPCONNECTION_H_ */
