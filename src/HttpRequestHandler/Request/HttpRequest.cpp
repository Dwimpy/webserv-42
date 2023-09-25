#include "HttpRequest.hpp"
#include "../../Parser/HttpRequestParser/HttpRequestParser.hpp"
#include <iostream>
#include <sstream>

HttpRequest::HttpRequest(const std::string &request): _versionMajor(1), _versionMinor(1)
{
	const char *str;
	str = request.c_str();

	HttpRequestParser::parseRequest(*this, str, str + request.size());
	HttpRequestParser::resetParser();
//	getFullBody();
}

HttpRequest::HttpRequest(): _versionMajor(1), _versionMinor(1)
{
	_parserState = StateRequestMethodStart;
	_parserResult = ParserDirectives;
}


HttpRequest::~HttpRequest()
{
}

void	HttpRequest::feedData(const char *str, ssize_t bytes)
{
	HttpRequestParser::setParserState(static_cast<ParserState>(_parserState));
	HttpRequestParser::setParserResult(static_cast<ParserResult>(_parserResult));
	HttpRequestParser::parseRequest(*this, str, str + bytes);
	_parserState = HttpRequestParser::getParserState();
	_parserResult = HttpRequestParser::getParserResult();
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

void	HttpRequest::pushToBody(char c)
{
	_body.__emplace_back(c);
}

std::string HttpRequest::getRequestBody()
{
	std::string s(_body.begin(), _body.end());
	return (s);
}

void	HttpRequest::pushHeader(const Header& header)
{
	_headers.push_back(header);
}

bool	HttpRequest::isEmptyHeader()
{
	return (this->_headers.empty());
}

size_t	HttpRequest::getHeadersSize()
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

std::string HttpRequest::getFullBody() const
{
	std::string str;
	size_t	idx;

	idx = 0;
	for (std::vector<char>::const_iterator it = _body.begin(); it < _body.end(); it++)
	{
		str.push_back(*it);
	}

	return (str);
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


const std::string HttpRequest::getValueByKey(const std::string &key) const
{
	for (size_t i = 0; i < _headers.size(); i++)
	{
		if (_headers[i].getKey() == key)
			return (_headers[i].getValue());
	}
	return ("");
}

std::string HttpRequest::getFileName()
{
	size_t	idx;

	if (getRequestUri() == "/")
		return ("index.html");
	idx = getRequestUri().find_last_of('.');
	std::string type = getRequestUri().substr(idx + 1);
	if (type == "css")
		return ("css/style.css");
	if (type == "jpeg" || type == "jpg")
		return ("assets/background.jpeg");
	if (type == "png")
		return ("assets_test/planet.png");
	if (type == "js")
		return ("js/script.js");
	return ("index.html");
}


const unsigned int &HttpRequest::getVersionMajor() const
{
	return (this->_versionMajor);
}

const unsigned int &HttpRequest::getVersionMinor() const
{
	return (this->_versionMinor);
}

const std::string &HttpRequest::getRequestUri() const
{
	return (this->_requestUri);
}
const std::string &HttpRequest::getRequestMethod() const
{
	return (this->_requestMethod);
}
