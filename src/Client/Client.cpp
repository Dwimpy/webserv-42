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

void	Client::send(const char *str, ssize_t size)
{
	size_t	offset;
	size_t	bufferSize;
	size_t	bytes_sent;

	bufferSize = sizeof(_clientSocket.getBuffer());
	offset = 0;
	while (size > 0)
	{
		_clientSocket.addToBuffer(str + offset, size);
		bytes_sent = _clientSocket.send();
		if (bytes_sent == 0)
			break;
		offset += bufferSize;
		size -= bufferSize;
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
