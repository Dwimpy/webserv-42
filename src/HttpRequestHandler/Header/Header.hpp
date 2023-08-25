#include <string>

class Header{
  public:
	Header(std::string key, std::string value);
	Header();
	~Header();
  private:
	std::string	_key;
	std::string	_value;
};