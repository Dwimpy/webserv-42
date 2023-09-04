#include "Server.hpp"

#include <sys/event.h>
#include <unistd.h>

static int flag = 0;

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
		std::cout << _buffer << std::endl;
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
	size_t	count;
	count = 0;
	if ((bytes_received = recv(client.getClientSocket().getFd(), _buffer, sizeof(_buffer) - 1, 0)) < 0)
		perror("Error receiving data");
	else if (bytes_received == 0)
		perror("ERROR connection closed by client");
	else
	{
		std::ofstream output("text.png");
		std::string response_msg = std::string(_buffer);
//		std::cout << _buffer << std::endl;
		HttpRequest request(response_msg);
//		std::cout << "\n\n\n\n" << request.getValueByKey("Content-Type") << std::endl;
//		std::cout << "EMPTY" << std::endl;
		std::string accept;
		if(request.getValueByKey("Content-Type").substr(0, 5) == "multi" || request.getValueByKey("Content-Type").substr(0, 5)  == "image" || flag == 1)
		{
			flag = 0;
			std::string newBuffer;
			accept = "HTTP/1.1 202 Accepted\r\n\r\n";
			send(client.getClientSocket().getFd(), accept.c_str(), accept.length(), 0);
			while (((bytes_received = recv(client.getClientSocket().getFd(), _buffer, sizeof(_buffer) - 1, 0)) > 0))
			{
//				if(!flag)
				newBuffer += std::string(_buffer);
				count += bytes_received;
				send(client.getClientSocket().getFd(), accept.c_str(), accept.length(), 0);
//				flag = 1;
			}
			newBuffer += std::string(_buffer);
			std::cout << newBuffer << std::endl;
			output << newBuffer;
			output.close();
			std::cout << request.getValueByKey("Content-Length") << std::endl;
			std::cout << "\n\n Bytes Received: " << count << std::endl;
//			send(client.getClientSocket().getFd(), newBuffer.c_str(), newBuffer.length(), 0);

		}
			else
			{
				HttpResponse responseObj(request, _config);
				std::string response = responseObj.getResponse();
				bytes_received = send(client.getClientSocket().getFd(), response.c_str(), response.size(), 0);
				if (bytes_received < 0)
					perror("ERROR socket closed");
			}
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
