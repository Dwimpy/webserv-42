#pragma once
#include "SocketHandler.hpp"
#include <string>
#include <chrono>
#include <random>

class Client{

  public:
	Client();
	~Client();
	void	generateSessionId(int length);
	const std::string &getClientSession() const;

  private:
	std::string	_sessionId;
	bool		_hasCookie;
	std::chrono::time_point<std::chrono::system_clock> _timeSinceUpdate;
};