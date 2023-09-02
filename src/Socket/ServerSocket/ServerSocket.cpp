#include "ServerSocket.hpp"
#include <sys/socket.h>
#include <unistd.h>

ServerSocket::ServerSocket()
{}

ServerSocket::~ServerSocket()
{}

bool ServerSocket::create(int port)
{
	try
	{
		memset(&this->_serverAddr, 0, sizeof(this->_serverAddr));
		this->_serverAddr.sin_family = AF_INET;
		this->_serverAddr.sin_addr.s_addr = INADDR_ANY;
		this->_serverAddr.sin_port = htons(port);
		this->_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (this->_fd == -1)
			throw std::runtime_error("Error creating server socket\n");
		int reuse = 1;
		setsockopt(this->_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
		fcntl(this->_fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
		std::cout << "Socket created" << std::endl;
		return (true);
	} catch (std::exception &e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return (false);
	}
}

bool ServerSocket::bind()
{
	try
	{
		if ((::bind(this->_fd, (t_sockaddr *) &this->_serverAddr, sizeof(this->_serverAddr)) < 0))
			throw std::runtime_error("Error binding to address\n");
		std::cout << "Binding complete" << std::endl;
		return (true);
	} catch (const std::exception &e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return (false);
	}
}

bool ServerSocket::listen()
{
	try
	{
		if ((::listen(this->_fd, 1024)) < 0)
			throw std::runtime_error("Error listening on Socket\n");
		return (true);
	} catch (std::exception &e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
		return (false);
	}
}

int &ServerSocket::getSocketFD()
{
	return (this->_fd);
}

int ServerSocket::accept(Client &client)
{
	int clientFd;
	struct timeval timeout;

	timeout.tv_sec = 0;
	timeout.tv_usec = 0;
	clientFd = ::accept(this->_fd, (t_sockaddr *)&client.getClientSocket().getClientAddr(), &client.getClientSocket().getClientAddrLen());
	setsockopt(clientFd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
	setsockopt(clientFd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
	client.getClientSocket().setFd(clientFd);
	return (clientFd);
}

void ServerSocket::close()
{
	::close(this->_fd);
}
