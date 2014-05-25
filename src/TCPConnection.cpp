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
#include <syslog.h>
#include <sys/select.h>

using namespace std;

TCPConnection::TCPConnection(const string& port, const string& max_cons, int sock_timeout) : port(port), sock_timeout(sock_timeout)
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

int TCPConnection::BindToAddress()
{
	err = bind(sockfd, addresstobind->ai_addr, addresstobind->ai_addrlen);
	if (err == -1)
		throw std::runtime_error(strerror(errno));

	err = listen(sockfd, max_connections);
	if (err == -1)
		throw std::runtime_error(strerror(errno));

	return err;
}

int TCPConnection::AcceptClient()
{
	syslog(LOG_INFO, "Attempting to accept client connection...");

	fd_set write_set;
	FD_ZERO(&write_set);
	FD_SET(sockfd, &write_set);

	timeval tval;
	tval.tv_sec = sock_timeout;
	tval.tv_usec = 0;

	int select_ret = 0;

	if ((select_ret = select(sockfd+1, &write_set, NULL, NULL, &tval)) > 0)
	{
		syslog(LOG_INFO, "Client found ready to connect.");
		int client_sock = accept(sockfd, NULL, NULL);

		if (client_sock == -1 && errno != EINTR)
			throw std::runtime_error(strerror(errno));

		return client_sock;
	}
	syslog(LOG_INFO, "No waiting clients found.");
	return -1;
}

TCPConnection::~TCPConnection() {

	err = close(sockfd);
	if (err == -1)
		perror("close");
}


