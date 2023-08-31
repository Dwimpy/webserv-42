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


void Server::run()
{
//	while (!shouldExit)
//	{
//		int pollingStatus = _socketHandler.pollIncomingRequests();
//
//		if (pollingStatus == 0 && shouldExit)
//			break ;
//		else if (pollingStatus == -1 || pollingStatus == 0)
//			continue ;
//
//		handleIncomingRequests();
//	}
//	close(_socketHandler.getServerSocket());
	_socketHandler.cleanUpRemainingConnections();
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
}

//void Server::handleIncomingRequests()
//{
//	size_t	i;
//    for (i = 0; i < _socketHandler.getActiveSocketsSize(); i++)
//	{
//		t_pollfd currentClient = _socketHandler.getClientAtIndex(i);
//		if (SocketHandler::isReventPolling(currentClient.revents))
//		{
//			if (_socketHandler.isFdServerSocket(currentClient.fd))
//				_socketHandler.acceptIncomingRequest();
//			else if (sendResponse(currentClient))
//				_socketHandler.removeClientAtIndexAndCloseFd(i);
//			else
//				continue ;
//		}
//		else if (SocketHandler::isReventError(currentClient.revents))
//		{
//			_socketHandler.removeClientAtIndexAndCloseFd(i);
//		}
//	}
//}

bool	Server::sendResponse(t_pollfd currentClient)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));
	if ((bytes_received = recv(currentClient.fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		perror("ERROR reading from socket");
		close(currentClient.fd);
		currentClient.fd = -1;
		currentClient.revents = 0;
		return (false);
	}
	else if (bytes_received == 0)
	{
		perror("ERROR socket closed");
		close(currentClient.fd);
		currentClient.fd = -1;
		currentClient.revents = 0;
		return (false);
	}
	std::string response_msg = std::string(_buffer);
	HttpRequest request(response_msg);
	HttpResponse responseObj(request, _config);
	std::string response = responseObj.getResponse();
	send(currentClient.fd, response.c_str(), response.size(), 0);
	close(currentClient.fd);
	currentClient.fd = -1;
	currentClient.revents = 0;
	return (true);
}

bool	Server::sendResponse(Client client)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));
	if ((bytes_received = recv(client.getClientSocket().getFd(), _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		perror("ERROR reading from socket");
		close(client.getClientSocket().getFd());
		return (false);
	}
	_buffer[bytes_received] = '\0';
	std::string response_msg = std::string(_buffer);
	HttpRequest request(response_msg);
	HttpResponse responseObj(request, _config);
	std::string response = responseObj.getResponse();
	send(client.getClientSocket().getFd(), response.c_str(), response.size(), 0);
	client.getClientSocket().getFd();
	return (true);
}

void	Server::handleIncomingRequests(t_pollfd *pfd, indexToPollMap &map)
{
    std::map<int, t_pollfd *>::iterator it;
    std::vector<Client>::iterator it2;
//	for (it = map[3].begin(); it != map[3].end();)
//	{
//		if (it->second->revents == POLLIN)
//		{
//			sendResponse(*it->second);
//			it->second->fd = -1;
//			it->second->revents = 0;
//		}
//		else
//			++it;
//	}
		if (pfd->revents == POLLIN)
		{
			sendResponse(*pfd);
			pfd->fd = -1;
			pfd->revents = 0;
		}

//	_connectedClients.clear();
    // Close sockets and clear the vector in one pass
}

void	Server::closeRemainingSockets()
{
	for (size_t i = 0; i < _connectedClients.size(); ++i) {
		_connectedClients[i].getClientSocket().close();
		_connectedClients[i].getClientSocket().setFd(-1);
    }
	_connectedClients.clear();
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
