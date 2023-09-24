#include "ClientSocket.hpp"
#include <iostream>

ClientSocket::ClientSocket(): _fd(-1), _clientAddrLen(sizeof(_clientAddr))
{}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::receive()
{
	ssize_t	bytes_received;
//	memset(_buffer, 0, sizeof(_buffer));
	bytes_received = recv(this->_fd, _buffer, sizeof(_buffer), 0);
	if (bytes_received <= 0)
		return (false);
	_buffer[bytes_received] = '\0';
	_bytes_received = bytes_received;
	return (true);
}

ssize_t ClientSocket::send(ssize_t bufferSize)
{
	ssize_t	bytes_sent;
	bytes_sent = ::send(this->_fd, _sendBuffer, bufferSize, 0);
	return (bytes_sent);
}

void ClientSocket::addToBuffer(const char *str, size_t size)
{
	std::memcpy(_sendBuffer, str, size);
}

unsigned char	*ClientSocket::getBuffer()
{
	return (this->_buffer);
}

ssize_t 	ClientSocket::getBytesReceived()
{
	return (this->_bytes_received);
}

t_sockaddr_in &ClientSocket::getClientAddr()
{
	return (this->_clientAddr);
}

socklen_t	&ClientSocket::getClientAddrLen()
{
	return (this->_clientAddrLen);
}

void ClientSocket::close()
{
	::close(this->_fd);
}
void ClientSocket::setFd(int fd)
{
	this->_fd = fd;

}

const int &ClientSocket::getFd() const
{
	return (this->_fd);
}
