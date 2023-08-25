#include "Server.hpp"
#include "HttpRequest.hpp"
#include "fcntl.h"
#include "unistd.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <vector>
#include "HttpResponse.hpp"

Server::Server() : _socketHandler(SocketHandler()), _config(ServerConfig())
{}

Server::Server(const ServerConfig &config) : _config(config)
{

}

Server::~Server()
{
//	close(_serverFd);
}

bool Server::startServer()
{
	initServerAddr();
	if (!_socketHandler.createSocket() ||
		!_socketHandler.bindSocket(serverAddr) ||
		!_socketHandler.listenForClientConnections(std::stoi(_config.getPort())))
		return (false);
	return (true);
}

void Server::initServerAddr()
{
	signal(SIGINT, signalHandler);
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(std::stoi(_config.getPort()));
}

void Server::run()
{
	while (true)
	{
		int pollingStatus = _socketHandler.pollIncomingRequests();

		if (pollingStatus == 0 && shouldExit)
			break ;
		else if (pollingStatus == -1 || pollingStatus == 0)
			continue ;

		handleIncomingRequests();
	}
//	close(_socketHandler.getServerSocket());
	_socketHandler.cleanUpRemainingConnections();
}

void Server::handleIncomingRequests()
{
	size_t	i;
    for (i = 0; i < _socketHandler.getActiveSocketsSize(); i++)
	{
		t_pollfd currentClient = _socketHandler.getClientAtIndex(i);
		if (SocketHandler::isReventPolling(currentClient.revents))
		{
			if (_socketHandler.isFdServerSocket(currentClient.fd))
				_socketHandler.acceptIncomingRequest();
			else if (sendResponse(currentClient))
				_socketHandler.removeClientAtIndexAndCloseFd(i);
			else
				continue ;
		}
		else if (SocketHandler::isReventError(currentClient.revents))
		{
			_socketHandler.removeClientAtIndexAndCloseFd(i);
			--i;
		}

	}
}

bool	Server::sendResponse(t_pollfd currentClient)
{
	memset(_buffer, 0, sizeof(_buffer));
	if ((recv(currentClient.fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		perror("ERROR reading from socket");
		close(currentClient.fd);
		return (false);
	}
	std::string response_msg = std::string(_buffer);
	HttpRequest request(response_msg);
	HttpResponse responseObj(request, _config);
	std::string response = responseObj.getResponse();
	send(currentClient.fd, response.c_str(), response.size(), 0);
	response.clear();
	close(currentClient.fd);
	return (true);
}

void	signalHandler(int signal)
{
	if (signal == SIGINT)
		shouldExit = true;
}