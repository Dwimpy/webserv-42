#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"

HttpRequest::HttpRequest(const std::string &request)
{
	const char *str;
	str = request.c_str();
	HttpRequestParser::parseRequest(*this, str, str + request.size());
	std::cout << this->getRequestMethod() << std::endl;
}

void	HttpRequest::pushToRequestMethod(char c)
{
	this->_requestMethod.push_back(c);
}


std::string HttpRequest::getRequestMethod()
{
	return (this->_requestMethod);
}

HttpRequest::~HttpRequest()
{

}
