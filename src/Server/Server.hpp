#pragma once

#include <netinet/in.h>
#include <vector>
#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include "ServerConfig.hpp"
#include "ServerSocket.hpp"
#include "SocketHandler.hpp"
#include "HttpRequestHandler.hpp"
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
	void	run(void);
	void	handleIncomingRequests(t_pollfd *pfd, indexToPollMap &map);
	void 	acceptIncomingConnections(std::vector<t_pollfd> &pollfds, indexToPollMap &map);
	bool	sendResponse(t_pollfd currentClient);
	bool	sendResponse(Client client);
	void	closeRemainingSockets();
	bool	findClient(std::string sessionId);
	const ConfigFile	&getConfiguration() const;
	ServerSocket getSocket() const;
	void	removeFd();

  private:
	char							_buffer[8192];
	SocketHandler					_socketHandler;
	const ServerConfig				_config;
	const ConfigFile					_configFile;
	ServerSocket					_serverSocket;
	std::vector<Client>				_connectedClients;
};
