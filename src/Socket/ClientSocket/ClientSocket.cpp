#include "ClientSocket.hpp"

ClientSocket::ClientSocket(): _fd(-1), _clientAddrLen(sizeof(_clientAddr))
{}

ClientSocket::~ClientSocket()
{

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
