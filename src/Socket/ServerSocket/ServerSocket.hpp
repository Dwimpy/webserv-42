#pragma once
#include <iostream>
#include "ISocket.hpp"
#include "Client.hpp"

class ServerSocket : public ISocket {

  public:
	ServerSocket();
	~ServerSocket();

	bool	create(int port);
	bool	bind();
	bool	listen();
	void	close();
	int accept(Client &client);

	int	&getSocketFD();
  private:
	int						_fd;
	t_sockaddr_in			_serverAddr;
};