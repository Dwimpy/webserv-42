#include "Server.hpp"
#include "HttpRequest.hpp"
#include "stdlib.h"
#include "fcntl.h"
#include "unistd.h"
#include <cstdio>
#include <iostream>
#include <fstream>
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
	close(_socketHandler.getServerSocket());
	_socketHandler.cleanUpRemainingConnections();
}

void Server::handleIncomingRequests()
{
    for (int i = 0; i < _socketHandler.getActiveSocketsSize(); i++) {
		t_pollfd currentClient = _socketHandler.getClientAtIndex(i);
		if (SocketHandler::isReventPolling(currentClient.revents)) {
			if (_socketHandler.isFdServerSocket(currentClient.fd))
				_socketHandler.acceptIncomingRequest();
			else {
				memset(_buffer, 0, sizeof(_buffer));
				if ((recv(currentClient.fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
				{
					perror("ERROR reading from socket");
					close(currentClient.fd);
					continue;
				}
				std::cout << _buffer << std::endl;
                std::string response;
                std::ifstream	infile("docs/index.html");
                if (!infile.good()){
                    perror("error opening file1");
                    exit (1);
                }
                response.append("HTTP/1.1 200 OK\r\n"
                                "Content-Type: text/html\r\n"
                                "\r\n");
                char c;
                while(infile.get(c))
                    response.push_back(c);
//				std::cout << _buffer << std::endl;
				std::string response_msg = std::string(_buffer);
				HttpRequest request(response_msg);
				send(currentClient.fd, response.c_str(), response.size(), 0);
				response.clear();
                close(currentClient.fd);
				_socketHandler.removeClientAtIndexAndCloseFd(i);
				--i;
			}
		}
		else if (SocketHandler::isReventError(currentClient.revents))
		{
			_socketHandler.removeClientAtIndexAndCloseFd(i);
			--i;
		}

	}
}

void	signalHandler(int signal)
{
	if (signal == SIGINT)
		shouldExit = true;
}