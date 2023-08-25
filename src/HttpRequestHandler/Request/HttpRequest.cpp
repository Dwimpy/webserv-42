#include "HttpRequest.hpp"
#include "HttpRequestParser.hpp"
#include <iostream>

HttpRequest::HttpRequest(const std::string &request)
{
	const char *str;
	str = request.c_str();
//	printf("In function: %s\n", str);
	HttpRequestParser::parseRequest(*this, str, str + request.size());
	HttpRequestParser::resetParser();
	std::cout << "Method: " << getRequestMethod() << std::endl;
	std::cout << "Request URI: " << getRequestUri() << std::endl;
	std::cout << "HTTP Version: " << getVersionMajor() << "." << getVersionMinor() << std::endl;
	for (size_t i = 0; i < _headers.size(); ++i)
	{
		std::cout << "Name: " << _headers[i].getKey() << "\t" "Value: " << _headers[i].getValue() << std::endl;
	}
}

HttpRequest::~HttpRequest()
{

}

void	HttpRequest::pushToRequestMethod(char c)
{
	this->_requestMethod.push_back(c);
}

void	HttpRequest::pushToRequestUri(char c)
{
	this->_requestUri.push_back(c);
}

void	HttpRequest::setVersionMajor(unsigned int version)
{
	this->_versionMajor = version;
}

void	HttpRequest::setVersionMinor(unsigned int version)
{
	this->_versionMinor = version;
}

void	HttpRequest::pushToLastHeaderKey(char c)
{
	_headers.back().pushToKey(c);
};

void	HttpRequest::pushToLastHeaderValue(char c)
{
	_headers.back().pushToValue(c);
}


void	HttpRequest::pushHeader(const Header& header)
{
	_headers.push_back(header);
}

const	bool	HttpRequest::isEmptyHeader()
{
	return (this->_headers.empty());
}

const	size_t	HttpRequest::getHeadersSize()
{
	return (this->_headers.size());
}

const std::string &HttpRequest::getRequestMethod()
{
	return (this->_requestMethod);
}

const std::string &HttpRequest::getRequestUri()
{
	return (this->_requestUri);
}

const unsigned int &HttpRequest::getVersionMajor()
{
	return (this->_versionMajor);
}

const unsigned int &HttpRequest::getVersionMinor()
{
	return (this->_versionMinor);
}

const std::string HttpRequest::getValueByKey(const std::string &key)
{
	for (size_t i = 0; i < _headers.size(); i++)
	{
		if (_headers[i].getKey() == key)
			return (_headers[i].getValue());
	}
	return ("");
}

std::string HttpRequest::getContentType()
{
	size_t	idx;
	idx = getRequestUri().find_last_of('.');
	std::string type = getRequestUri().substr(idx + 1);
	return (type);
}