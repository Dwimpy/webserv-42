#pragma once
#include "ClientSocket.hpp"
#include <string>
#include <chrono>
#include <random>

class Client{

  public:
	Client();
	~Client();
	const std::string &getClientSession() const;

	const ClientSocket	&getClientSocket() const;

	void	setClientFd(int fd);
	void	setClientFd(int fd) const;
	void	setServer(int fd);
	ClientSocket 	&getClientSocket();
	static std::string	generateCookieId(int length);

	std::string			recieve();
	void				send(const char *str, ssize_t size);
	int					&getAssignedServer();

  private:
	std::string		_sessionId;
	bool			_hasCookie;
	ClientSocket	_clientSocket;
	int				_assignedServer;
	std::chrono::time_point<std::chrono::system_clock> _timeSinceUpdate;

	void	generateSessionId(int length);
};
