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
		std::cout << "Connection Accepted. Assigned to FD: " << clientFd << "\n";
		this->_connectedClients.__emplace_back(newClient);
		i++;
	}
}

void	Server::removeClient()
{
	for (std::vector<Client>::iterator it = _connectedClients.begin(); it != _connectedClients.end();)
	{
		if (it->getClientSocket().getFd() == -1)
		{
			it = _connectedClients.erase(it);
		}
		else
			++it;
	}
}

void	Server::sendResponse(int fd)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));

	if ((bytes_received = recv(fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		perror("Error receiving data");
		std::cout << "SERVER RECV: fd: " << fd << std::endl;
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
			perror("ERROR socket closed");
	}
}


void	Server::sendResponse(Client client)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));

	if ((bytes_received = recv(client.getClientSocket().getFd(), _buffer, sizeof(_buffer) - 1, 0)) < 0)
		perror("Error receiving data");
	else if (bytes_received == 0)
		perror("ERROR connection closed by client");
	else
	{
//		std::cout << _buffer << std::endl;
		std::string response_msg = std::string(_buffer);
		HttpRequest request(response_msg);
		std::cout << request.getValueByKey("key1");
		HttpResponse responseObj(request, _config);
		std::string response = responseObj.getResponse();
		bytes_received = send(client.getClientSocket().getFd(), response.c_str(), response.size(), 0);
		if (bytes_received < 0)
			perror("ERROR socket closed");
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
std::vector<Client> &Server::getConnectedClients()
{
	return (this->_connectedClients);
}
const ConfigFile &Server::getConfiguration()
{
	return (this->_configFile);
}
