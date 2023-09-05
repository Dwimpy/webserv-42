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
	ClientSocket 	&getClientSocket();
	std::string			recieve();
	static std::string	generateCookieId(int length);

  private:
	std::string		_sessionId;
	bool			_hasCookie;
	ClientSocket	_clientSocket;
	std::chrono::time_point<std::chrono::system_clock> _timeSinceUpdate;

	void	generateSessionId(int length);
};
