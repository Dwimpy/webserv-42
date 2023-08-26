#pragma once
#include <string>

class Header{
  public:
	Header(std::string key, std::string value);
	Header();
	~Header();
	void	pushToKey(char c);
	void	pushToValue(char c);

	const std::string &getKey();
	const std::string &getValue();

	const std::string &getKey() const;
	const std::string &getValue() const;

  private:
	std::string	_key;
	std::string	_value;
};