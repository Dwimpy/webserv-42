#include "ClientSocket.hpp"
#include <iostream>

ClientSocket::ClientSocket(): _data_offset(0), _fd(-1), _clientAddrLen(sizeof(_clientAddr))
{}

ClientSocket::~ClientSocket()
{

}

ssize_t ClientSocket::receive()
{
	ssize_t	bytes_received;

	memset(_buffer, 0, sizeof(_buffer));
	bytes_received = recv(this->_fd, _buffer, sizeof(_buffer), 0);
	this->_buffer[bytes_received] = '\0';
	return (bytes_received);
}

void	ClientSocket::setOffset(ssize_t n)
{
	this->_data_offset += n;
}

ssize_t	ClientSocket::getOffset()
{
	return (this->_data_offset);
}

ssize_t ClientSocket::send(const char *str, ssize_t size)
{
	ssize_t	bytes_sent;
	bytes_sent = ::send(this->_fd, str, size, 0);
	return (bytes_sent);
}

void ClientSocket::addToBuffer(const char *str, size_t size)
{
	std::memcpy(_buffer, str, size);
}

char	*ClientSocket::getBuffer()
{
	return (this->_buffer);
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
