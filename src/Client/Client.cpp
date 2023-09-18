#include "Client.hpp"

#include <sstream>
Client::Client():_clientSocket(ClientSocket()), _hasCookie(false), _timeSinceUpdate(std::chrono::system_clock::now())
{
	generateSessionId(64);
}

Client::~Client(){}

void	Client::generateSessionId(int length)
{
	const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::random_device					rd;
	std::mt19937						generator(rd());
	std::uniform_int_distribution<int>	distribution(0, chars.length() - 1);


	for (int i =0; i < length; ++i)
		this->_sessionId += chars[distribution(generator)];
}

void Client::setServer(int fd)
{
	this->_assignedServer = fd;
}

int &Client::getAssignedServer()
{
	return (this->_assignedServer);
}

void Client::send(const char *str, ssize_t size)
{
    ssize_t offset = 0;
    ssize_t bufferSize = 1024; // Choose an appropriate buffer size
    ssize_t bytes_sent = 0;

    while (size > 0)
    {
        if (size < bufferSize)
            bufferSize = size; // Send the remaining data if it's smaller than the buffer size
        _clientSocket.addToBuffer(str + offset, bufferSize);
       	bytes_sent = _clientSocket.send(bufferSize);

        if (bytes_sent <= 0)
            break ;

        offset += bytes_sent;
        size -= bytes_sent;
    }
}


std::string	Client::generateCookieId(int length)
{
	const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::random_device					rd;
	std::mt19937						generator(rd());
	std::uniform_int_distribution<int>	distribution(0, chars.length() - 1);
	std::string                         sessionId;

	for (int i =0; i < length; ++i)
		sessionId += chars[distribution(generator)];
	return(sessionId);
}

std::string Client::recieve()
{
	std::ostringstream os;

	while (_clientSocket.receive())
		os << std::string(_clientSocket.getBuffer());
	return (os.str());
}

const std::string &Client::getClientSession() const
{
	return (this->_sessionId);
}

const ClientSocket &Client::getClientSocket() const
{
	return (this->_clientSocket);
}

ClientSocket &Client::getClientSocket()
{
	return (this->_clientSocket);
}

void Client::setClientFd(int fd)
{
	this->_clientSocket.setFd(fd);
}
