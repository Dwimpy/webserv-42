#include "Server.hpp"
#include <unistd.h>

Server::Server() : _socketHandler(SocketHandler()), _config(ServerConfig())
{}

Server::Server(const ServerConfig &config) : _config(config)
{}

Server::Server(const ConfigFile &config): _socketHandler(SocketHandler()), _configFile(config), _config(ServerConfig())
{}

Server::~Server()
{
//	close(_serverFd);
}

bool Server::startServer()
{
	if (!_serverSocket.create(_configFile.getPort())||
		!_serverSocket.bind() ||
		!_serverSocket.listen())
		return (false);
	return (true);
}


void Server::acceptIncomingConnections(std::vector<t_pollfd> &pollfds, indexToPollMap &map)
{
	int		clientFd;

	clientFd = 0;
	while (true)
	{
		Client	newClient;
		clientFd = _serverSocket.accept(newClient);
		if (clientFd == -1)
			break ;
		this->_connectedClients.__emplace_back(newClient);
		pollfds.__emplace_back((t_pollfd){clientFd, POLLIN, 0});
		(map)[_serverSocket.getSocketFD()][clientFd] = &pollfds.back();
	}
	_connectedClients.clear();
}

bool	Server::sendResponse(t_pollfd currentClient)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));
	if ((bytes_received = recv(currentClient.fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		perror("ERROR reading from socket");
		close(currentClient.fd);
		return (false);
	}
	else if (bytes_received == 0)
	{
		perror("ERROR socket closed");
		close(currentClient.fd);
		return (false);
	}
	std::string response_msg = std::string(_buffer);
	HttpRequest request(response_msg);
	HttpResponse responseObj(request, _config);
	std::string response = responseObj.getResponse();
	send(currentClient.fd, response.c_str(), response.size(), 0);
	close(currentClient.fd);
	return (true);
}

void	Server::handleIncomingRequests(indexToPollMap &map)
{
    std::map<int, t_pollfd *>::iterator it;

	for (it = map[this->_serverSocket.getSocketFD()].begin(); it != map[this->_serverSocket.getSocketFD()].end();)
	{
		if (it->second->revents == POLLIN)
		{
			sendResponse(*it->second);
			it->second->fd = -1;
			it->second->revents = 0;
		}
		else
			it++;
	}
}

void Server::removeFd()
{
	std::vector<Client>::iterator it;

	for (it = _connectedClients.begin(); it != _connectedClients.end(); )
	{
		if (it->getClientSocket().getFd() == -1)
			it = _connectedClients.erase(it);
		else
			++it;
	}
}

const ConfigFile &Server::getConfiguration() const
{
	return (this->_configFile);
}

ServerSocket Server::getSocket() const
{
	return (this->_serverSocket);
}
