#pragma once

#include "ServerSocket.hpp"
#include "ConfigFile.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;
typedef std::map<int, std::map<int, t_pollfd *> > indexToPollMap;

class Server {

  public:
	Server();
	Server(const ConfigFile &config);
	~Server();
	bool	startServer();

	void	acceptIncomingConnections(int &kq);
	const ConfigFile		&getConfiguration() const;
	const ConfigFile			&getConfiguration();
	std::vector<Client *> &getConnectedClients();
	ServerSocket		getSocket() const;
	void				removeClient(Client &client);

  private:
	ConfigFile						_configFile;
	ServerSocket					_serverSocket;
	std::vector<Client *>			_connectedClients;
};
