#pragma once
#include "SocketHandler.hpp"
#include "ClientSocket.hpp"
#include <string>
#include <chrono>
#include <random>

class Client{

  public:
	Client();
	~Client();
	const std::string &getClientSession() const;

	ClientSocket	&getClientSocket();

  private:
	std::string		_sessionId;
	bool			_hasCookie;
	ClientSocket	_clientSocket;
	std::chrono::time_point<std::chrono::system_clock> _timeSinceUpdate;

	void	generateSessionId(int length);
};
