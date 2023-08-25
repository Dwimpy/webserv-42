#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Header.hpp"

class HttpRequestParser;

class HttpRequest {

	typedef std::vector<Header> request_headers;
	typedef std::vector<char>	request_body;

  public:
	HttpRequest(const std::string &request);
	~HttpRequest();
	void	pushToRequestMethod(char c);
	void	pushToRequestUri(char c);
	void	pushHeader(const Header& header);
	void	setVersionMajor(unsigned int version);
	void	setVersionMinor(unsigned int version);
	void	pushToLastHeaderKey(char c);
	void	pushToLastHeaderValue(char c);

	const size_t		getHeadersSize();
	const bool			isEmptyHeader();
	const std::string	getValueByKey(const std::string &key);
	const std::string	&getRequestMethod();
	const std::string	&getRequestUri();
	std::string			getContentType();
	std::string			getFileName();
	const unsigned int	&getVersionMajor();
	const unsigned int	&getVersionMinor();


  private:
	std::string			_requestMethod;
  	std::string			_requestUri;
	unsigned int		_versionMinor;
	unsigned int		_versionMajor;
	request_headers 	_headers;
	request_body		_body;
};