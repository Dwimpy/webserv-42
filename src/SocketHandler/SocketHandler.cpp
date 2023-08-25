#include "SocketHandler.hpp"
#include <cstdio>
#include <iostream>



SocketHandler::SocketHandler() : _serverSocket(-1), _pollReady(-1)
{}

SocketHandler::~SocketHandler()
{}

bool	SocketHandler::createSocket()
{
	try {
		this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_serverSocket == -1)
			throw std::runtime_error("Error creating server socket\n");
		std::cout <<"Socket created" << std::endl;
		return (true);
	} catch(std::exception &e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return (false);
	}
}

bool	SocketHandler::bindSocket(const t_sockaddr_in &serverAddr) const
{
	try {
		if ((bind(_serverSocket, (t_sockaddr *)&serverAddr, sizeof(serverAddr)) < 0))
			throw std::runtime_error("Error binding to address\n");
		std::cout << "Binding complete" << std::endl;
		return (true);
	} catch (const std::exception &e){
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return (false);
	}
}

bool	SocketHandler::listenForClientConnections(const unsigned int &port)
{
	try {
		if ((listen(_serverSocket, 5)) < 0)
			throw std::runtime_error("Error listening on Socket\n");
		std::cout << "Server listening on port: " << port << std::endl;
		_activeSockets.__emplace_back((t_pollfd){_serverSocket, POLLIN, 0});
		return (true);
	} catch(std::exception &e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return (false);
	}
}

int	SocketHandler::pollIncomingRequests()
{
	const int	timeout = 2000;

	try {
		this->_pollReady = poll(_activeSockets.data(), _activeSockets.size(), timeout);
		if (this->_pollReady < 0 && errno != EINTR)
			throw std::runtime_error("Polling encountered an error");
		else if (this->_pollReady == 0)
			return (0);
		return (1);
	} catch (std::exception &e){
		std::cerr << "Caught exception: " << e.what() << std::flush;
		return (-1);
	}
}

int SocketHandler::acceptIncomingRequest()
{
	try{
		socklen_t		clientAddrLen;
		t_sockaddr_in	clientAddr;
		clientAddrLen = sizeof(clientAddr);
		const int	clientSocket = accept(_serverSocket, (t_sockaddr *)&clientAddr, &clientAddrLen);
		if (clientSocket < 0)
			throw std::runtime_error("Error accepting incoming client\n");
		if (_connectedClients.insert(clientSocket).second)
        {
            std::cout << clientSocket << std::endl;
			_activeSockets.push_back((t_pollfd){clientSocket, POLLIN, 0});
        }
		return (true);
	}catch (std::exception &e) {
		std::cerr << "Exception caught: " << e.what();
		return (false);
	}
}


void	SocketHandler::removeClientAtIndexAndCloseFd(size_t index)
{
	std::vector<t_pollfd >::iterator it = _activeSockets.begin() + static_cast<long>(index);

	if (it != _activeSockets.end()){
		_connectedClients.erase(getClientAtIndex(index).fd);
		_activeSockets.erase(it);
	}
	else{
		std::cerr << "Iterator out of bounds\n" << std::endl;
	}
}

bool	SocketHandler::isReventPolling(const short &revents)
{
	if (revents & POLLIN)
		return (true);
	return (false);
}

bool	SocketHandler::isFdServerSocket(const int &fd)
{
	if (fd == _serverSocket)
		return (true);
	return (false);
}

const int	&SocketHandler::getServerSocket() const
{
	return (this->_serverSocket);
}

const t_pollfd	&SocketHandler::getClientAtIndex(size_t index) const
{
	return (this->_activeSockets[index]);
}

void	SocketHandler::cleanUpRemainingConnections()
{
	std::vector<t_pollfd >::iterator it = _activeSockets.begin();
	if (it != _activeSockets.end()){
			close(it->fd);
			_connectedClients.erase(it->fd);
	}
	else{
		std::cerr << "Iterator out of bounds\n" << std::endl;
	}
	_activeSockets.clear();
}

size_t	SocketHandler::getActiveSocketsSize()
{
	return (_activeSockets.size());
}

const int	&SocketHandler::getPollingStatus() const
{
	return (this->_pollReady);
}

bool	SocketHandler::isReventError(const short &revents)
{
	return (revents & POLLHUP || revents & POLLERR);
}
