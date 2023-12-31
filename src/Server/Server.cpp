#include "Server.hpp"

#include <sys/event.h>
#include <unistd.h>
#include <algorithm>

Server::Server(const ConfigFile &config): _configFile(config) {}


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


void Server::acceptIncomingConnections(int &kq)
{
	int		clientFd;

	while (true)
	{
		Client	*newClient = new Client();
		clientFd = _serverSocket.accept(*newClient);
		if (clientFd < 0)
		{
			delete newClient;
			break ;
		}
		newClient->setServer(this->_serverSocket.getSocketFD() - 3);
		this->_connectedClients.push_back(newClient);
		struct kevent	read_event;
		EV_SET(&read_event, newClient->getClientSocket().getFd(), EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, newClient);
		kevent(kq, &read_event, 1, nullptr, 0, nullptr);
		std::cout << "Connection Accepted" << std::endl;
	}
}

void	Server::removeClient(Client &client)
{
    for (std::vector<Client *>::iterator it = _connectedClients.begin(); it != _connectedClients.end();) {

        if ((*it)->getClientSocket().getFd() == client.getClientSocket().getFd()) {
			delete *it;
            _connectedClients.erase(it);
			return ;
        }
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
std::vector<Client *> &Server::getConnectedClients()
{
	return (this->_connectedClients);
}
const ConfigFile &Server::getConfiguration()
{
	return (this->_configFile);
}
