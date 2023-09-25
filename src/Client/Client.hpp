#pragma once
#include "ClientSocket.hpp"
#include "HttpRequest.hpp"

#include <chrono>
#include <random>
#include <sstream>
#include <string>

class Client{

  public:
	Client();
	~Client();
	const std::string &getClientSession() const;

	const ClientSocket	&getClientSocket() const;

	void	setClientFd(int fd);
	void	setServer(int fd);
	ClientSocket 	&getClientSocket();
	const HttpRequest &getRequest();
	static std::string	generateCookieId(int length);

	void				setHasClosed();
	void				recieve();
	void				send(const char *str, ssize_t size);
	int					&getAssignedServer();

  private:
	std::string					_sessionId;
	bool						_hasCookie;
	ClientSocket				_clientSocket;
	int							_assignedServer;
	HttpRequest					_request;
	std::chrono::time_point<std::chrono::system_clock> _timeSinceUpdate;
	bool						_hasClosed;
	void	generateSessionId(int length);
};
