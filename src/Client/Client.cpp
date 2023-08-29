#include "Client.hpp"

Client::Client(): _hasCookie(false), _timeSinceUpdate(std::chrono::system_clock::now())
{
	generateSessionId(64);
}

Client::~Client(){}

void	Client::generateSessionId(int length)
{
	const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::random_device					rd;
	std::mt19937						generator(rd());
	std::uniform_int_distribution<int>	distribution(0, chars.length() - 1);


	for (int i =0; i < length; ++i)
		this->_sessionId += chars[distribution(generator)];
}

const std::string &Client::getClientSession() const
{
	return (this->_sessionId);
}