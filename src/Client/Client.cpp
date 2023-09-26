#include "Client.hpp"
#include "HttpRequestParser.hpp"

#include <iostream>
#include <sstream>
Client::Client():_sendComplete(false), _closeConnection(false), _clientSocket(ClientSocket()), _hasCookie(false), _timeSinceUpdate(std::chrono::system_clock::now())
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
   ssize_t	bytes_sent;

   bytes_sent = this->_clientSocket.send(str + this->_clientSocket.getOffset(), size - this->_clientSocket.getOffset());
   if (bytes_sent > 0)
   {
		this->_clientSocket.setOffset(bytes_sent);
		if (bytes_sent != size)
		{
			send(str, size);
			return ;
		}
		_closeConnection = true;
		_sendComplete = true;
   } else if (bytes_sent < 0 && errno == EAGAIN)
		return ;
}


bool	Client::hasClosed()
{
	return (this->_closeConnection);
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

void Client::recieve()
{
	ssize_t bytes_recv;
	bytes_recv = _clientSocket.receive();
	if (bytes_recv > 0)
	{
		_request.feedData(_clientSocket.getBuffer(), bytes_recv);
		recieve();
		return ;
	}
	else if (bytes_recv == 0)
	{
		std::cerr << "Client has disconnected" << std::endl;
		_closeConnection = true;
	}
	else
	{
		if (errno != EWOULDBLOCK)
			_closeConnection = true;
		return;
	}
}

int Client::getParserResult() const
{
	return (this->_request.getParserResult());
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
const HttpRequest &Client::getRequest() { return this->_request; }
bool Client::isSendComplete() { return this->_sendComplete; }
