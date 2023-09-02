#include "Server.hpp"

#include <sys/event.h>
#include <unistd.h>

Server::Server() : _config(ServerConfig())
{}

Server::Server(const ServerConfig &config) : _config(config)
{}

Server::Server(const ConfigFile &config): _configFile(config), _config(ServerConfig())
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
		std::cout << "Connection Accepted" << std::endl;
	}
}

void Server::acceptIncomingConnections(int kq, struct kevent change[25])
{
	int		clientFd;
	size_t	i;

	i = 0;
	clientFd = 0;
//	this->removeClient();
	while (true)
	{
		Client	newClient;
		clientFd = _serverSocket.accept(newClient);
		if (clientFd == -1)
			break ;
		this->_connectedClients.__emplace_back(newClient);
		i++;
	}
}

void	Server::removeClient()
{
	for (std::vector<Client>::iterator it = _connectedClients.begin(); it != _connectedClients.end();)
	{
		if (it->getClientSocket().getFd() == -1)
			it = _connectedClients.erase(it);
		else
			++it;
	}
}

void Server::acceptIncomingConnections(std::vector<t_pollfd> &pollfds, std::map<t_pollfd *, int> &map)
{
	int		clientFd;

	clientFd = 0;
	while (true)
	{
		Client	newClient;
		clientFd = _serverSocket.accept(newClient);
		if (clientFd < 0)
		{
			if (errno == EWOULDBLOCK)
				std::cerr << "Accept timed out" << std::endl;
			else
				break ;
		}
		else
		{
			this->_connectedClients.__emplace_back(newClient);
			pollfds.__emplace_back((t_pollfd){clientFd, POLLIN, 0});
			map[&pollfds.back()] = this->_serverSocket.getSocketFD() - 3;
		}
	}
}

bool	Server::sendResponse(t_pollfd currentClient)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));
	if ((bytes_received = recv(currentClient.fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		close(currentClient.fd);
		perror("Error receiving data");
	}
	else if (bytes_received == 0)
	{
		perror("ERROR connection closed by client");
		close(currentClient.fd);
	}
	else
	{
		std::string response_msg = std::string(_buffer);
		HttpRequest request(response_msg);
		std::cout << _buffer << std::endl;
		HttpResponse responseObj(request, _config);
		std::string response = responseObj.getResponse();
		bytes_received = send(currentClient.fd, response.c_str(), response.size(), 0);
		if (bytes_received < 0)
		{
			perror("ERROR socket closed");
			close(currentClient.fd);
			return (false);
		}
		close(currentClient.fd);
		return (true);
	}
	return (false);
}

bool	Server::sendResponse(int fd)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));
	if ((bytes_received = recv(fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		perror("Error receiving data");
	}
	else if (bytes_received == 0)
	{
		perror("ERROR connection closed by client");
	}
	else
	{
//		std::cout << _buffer << std::endl;
		std::string response_msg = std::string(_buffer);
		HttpRequest request(response_msg);
		std::cout << request.getValueByKey("key1");
		HttpResponse responseObj(request, _config);
		std::string response = responseObj.getResponse();
		bytes_received = send(fd, response.c_str(), response.size(), 0);
		if (bytes_received < 0)
		{
			perror("ERROR socket closed");

			return (false);
		}
		return (true);
	}
	return (false);
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
	else if (bytes_received == 0)
	{
		perror("ERROR socket closed");
		close(client.getClientSocket().getFd());
		return (false);
	}
    std::cout << "\033[0;92m" << _buffer << "\033[0;39m" << std::endl;
	std::string response_msg = std::string(_buffer);
	HttpRequest request(response_msg);
	HttpResponse responseObj(request, _config);
	std::string response = responseObj.getResponse();
	send(client.getClientSocket().getFd(), response.c_str(), response.size(), 0);
	close(client.getClientSocket().getFd());
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
		it++;
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
