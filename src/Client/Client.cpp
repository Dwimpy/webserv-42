#include "Client.hpp"

Client::Client(): _clientSocket(ClientSocket()), _hasCookie(false), _timeSinceUpdate(std::chrono::system_clock::now())
{
	this->_sessionId = generateSessionId(64);
}

Client::~Client(){}

std::string	Client::generateCookieId(int length)
{
	std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::random_device					rd;
	std::mt19937						generator(rd());
	std::uniform_int_distribution<int>	distribution(0, chars.length() - 1);
	std::string                         sessionId;

	for (int i =0; i < length; ++i)
		sessionId += chars[distribution(generator)];
	return(sessionId);
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

std::string	generateSessionId(int length)
{
	const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::string sessionId;

	srand((unsigned)time(NULL) * getpid());
	for (int i =0; i < length; ++i)
		sessionId += chars[rand() % (chars.length() - 1)];
	return (sessionId);
}