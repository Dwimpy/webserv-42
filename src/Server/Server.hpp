#pragma once

#include <netinet/in.h>
#include <vector>
#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include "ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "ConfigFile.hpp"
#include "HttpRequest.hpp"
#include "unistd.h"
#include "HttpResponse.hpp"
#include "ServerSocket.hpp"

typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;
typedef std::map<int, std::map<int, t_pollfd *> > indexToPollMap;

class Server {

  public:
	Server();
	Server(const ServerConfig &config);
	Server(const ConfigFile &config);
	~Server();
	bool	startServer();

	void	acceptIncomingConnections(int kq, struct kevent change[25]);
	bool	sendResponse(Client client);
	bool	sendResponse(int fd);

	const ConfigFile	&getConfiguration() const;
	std::vector<Client> &getConnectedClients();
	ServerSocket getSocket() const;
	void	removeFd();
	void	removeClient();

  private:
	char							_buffer[8192];
	const ServerConfig				_config;
	const ConfigFile					_configFile;
	ServerSocket					_serverSocket;
	std::vector<Client>				_connectedClients;
};
