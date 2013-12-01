/*
 * TCPConnection.cpp
 *
 *  Created on: 22 Sep 2013
 *      Author: ieuan
 */

#include "TCPConnection.h"
#include <string>
#include <sstream>
#include "Helpers.h"
#include "StringOperations.h"

using namespace std;

TCPConnection::TCPConnection(string port, string max_cons) : port(port)
{
	addinfo *addrlist;
	addinfo hints;

	max_connections = StringOperations::StringToInt(max_cons);

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	err = getaddrinfo(NULL, port.c_str(), &hints, &addrlist);
	if (err != 0)
		throw std::runtime_error(gai_strerror(err));

	addresstobind = addrlist;
	sockfd = socket(addrlist->ai_family, addrlist->ai_socktype, addrlist->ai_protocol);

	int val = 1;
	err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
	if (err == -1)
		throw std::runtime_error(strerror(errno));
}

int TCPConnection::bindtoaddr()
{
	err = bind(sockfd, addresstobind->ai_addr, addresstobind->ai_addrlen);
	if (err == -1)
		throw std::runtime_error(strerror(errno));

	err = listen(sockfd, max_connections);
	if (err == -1)
		throw std::runtime_error(strerror(errno));

	return err;
}

int TCPConnection::getclientsock()
{
	int clientsockfd = accept(sockfd, NULL, NULL);
	if (err == -1)
		throw std::runtime_error(strerror(errno));

	return clientsockfd;
}

TCPConnection::~TCPConnection() {

	err = close(sockfd);
	if (err == -1)
		perror("close");
}


