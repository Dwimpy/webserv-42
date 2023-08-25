#include "Header.hpp"

Header::Header(std::string key, std::string value): _key(key), _value(value) {}
Header::Header(): _key(std::string("")), _value(std::string("")){}
Header::~Header() {}

void	Header::pushToKey(char c)
{
	this->_key.push_back(c);
}

void	Header::pushToValue(char c)
{
	this->_value.push_back(c);
}

const std::string &Header::getKey()
{
	return (this->_key);
}

const std::string &Header::getValue()
{
	return (this->_value);
}