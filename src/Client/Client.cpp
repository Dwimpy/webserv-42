#include "Client.hpp"
#include "HttpRequestParser.hpp"

#include <iostream>
#include <sstream>
Client::Client():_closeConnection(false), _clientSocket(ClientSocket()), _hasCookie(false), _timeSinceUpdate(std::chrono::system_clock::now())
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
	close(this->_clientSocket.getFd());
}


bool	Client::getHasClosed()
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
