#include "ClientSocket.hpp"

ClientSocket::ClientSocket(): _fd(-1), _clientAddrLen(sizeof(_clientAddr))
{}

ClientSocket::~ClientSocket()
{

}

bool ClientSocket::receive()
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));
	bytes_received = recv(this->_fd, _buffer, sizeof(_buffer), 0);
	if (bytes_received <= 0)
		return (false);
	_buffer[bytes_received] = '\0';
	return (true);
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
