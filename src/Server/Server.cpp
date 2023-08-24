#include "Server.hpp"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include <cstdio>
#include <iostream>
#include <signal.h>
#include <sys/socket.h>
#include <vector>

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
		!_socketHandler.bindSocket(_socketContext.serverAddr) ||
		!_socketHandler.listenForClientConnections(std::stoi(_config.getPort())))
		return (false);
	return (true);
}

void Server::initServerAddr()
{
	signal(SIGINT, signalHandler);
	memset(&_socketContext.serverAddr, 0, sizeof(_socketContext.serverAddr));
	_socketContext.serverAddr.sin_family = AF_INET;
	_socketContext.serverAddr.sin_addr.s_addr = INADDR_ANY;
	_socketContext.serverAddr.sin_port = htons(std::stoi(_config.getPort()));
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
	close(_socketHandler.getServerSocket());
	_socketHandler.cleanUpRemainingConnections();
}

void Server::handleIncomingRequests()
{
	for (int i = 0; i < _socketHandler.getActiveSocketsSize(); i++) {
		t_pollfd currentClient = _socketHandler.getClientAtIndex(i);
		if (SocketHandler::isReventPolling(currentClient.revents)) {
			if (_socketHandler.isFdServerSocket(currentClient.fd)) {
				_socketHandler.acceptIncomingRequest();
			}
			else {
				memset(_buffer, 0, sizeof(_buffer));
				if ((recv(currentClient.fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
				{
					perror("ERROR reading from socket");
					close(currentClient.fd);
					continue;
				}
				std::cout << _buffer << std::endl;
				const char *response = "HTTP/1.0 200 OK\r\n"
									   "Content-Type: text/html\r\n"
									   "\r\n"
									   "<html><body><h1>WRONG! Start again!</h1> <a class=\"nav__link\" href=\"../../docs/index.html\">Home</a></body></html>";
				send(currentClient.fd, response, strlen(response), 0);
				close(currentClient.fd);
				_socketHandler.removeClientAtIndexAndCloseFd(i);
				--i;
			}
		}
		else if (SocketHandler::isReventError(currentClient.revents))
		{
			_socketHandler.removeClientAtIndexAndCloseFd(i);
			i--;
		}

	}
}

//	while (!shouldExit)
//	{
//		_clientLen = sizeof(_clientAddrs);
//		// Accept a new connection
//		_clientFd = accept(_serverFd, (t_sockaddr *)&_clientAddrs, &_clientLen);
//		if (_clientFd < 0)
//		{
//			perror("ERROR on accept");
//			continue;
//		}
//
//		// Read the request from the client
//		memset(_buffer, 0, sizeof(_buffer));
//		_receivedBytes = recv(_clientFd, _buffer, sizeof(_buffer) - 1, 0);
//		if (_receivedBytes < 0)
//		{
//			perror("ERROR reading from socket");
//			close(_clientFd);
//			continue;
//		}
//		std::cout << _buffer << std::endl;
//		// Send a simple HTTP response
//		const char *response = "HTTP/1.0 200 OK\r\n"
//							   "Content-Type: text/html\r\n"
//							   "\r\n"
//							   "<html><body><h1>WRONG! Start again!</h1> <a class=\"nav__link\" href=\"../../docs/index.html\">Home</a></body></html>";
//		send(_clientFd, response, strlen(response), 0);
//
//		// Close the client socket
//		close(_clientFd);
//	}
//	close(_serverFd);
//}

void	signalHandler(int signal)
{
	if (signal == SIGINT)
		shouldExit = true;
}