#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "Header.hpp"

class HttpRequest {

	typedef std::vector<Header> request_headers;
	typedef std::vector<char>	request_body;

  public:
	HttpRequest(const std::string &request);
	HttpRequest();
	~HttpRequest();
	void	pushToRequestMethod(char c);
	void	pushToRequestUri(char c);
	void	pushHeader(const Header& header);
	void	setVersionMajor(unsigned int version);
	void	setVersionMinor(unsigned int version);
	void	pushToLastHeaderKey(char c);
	void	pushToLastHeaderValue(char c);
	void	pushBodyHeader(const Header& header);
	void	pushToLastBodyHeaderKey(char c);
	void	pushToLastBodyHeaderValue(char c);
	void	pushToBody(char c);

	size_t		getHeadersSize();
	bool			isEmptyHeader();
	const std::string	getValueByKey(const std::string &key);
	const std::string	&getRequestMethod();
	const std::string	&getRequestUri();
	std::string			getContentType();
	std::string			getFileName();
	const unsigned int	&getVersionMajor();
	const unsigned int	&getVersionMinor();
	std::string			getFullBody() const;

	std::string			getFileName() const;
	std::string			getContentType() const;
	const std::string	getValueByKey(const std::string &key) const;
	const std::vector<Header>	&getBody(const std::string &key) const;
	const std::string	&getRequestUri() const;
	const std::string	&getRequestMethod() const;
	std::string			getRequestBody();
	const unsigned int	&getVersionMajor() const;
	const unsigned int	&getVersionMinor() const;
	void				feedData(const char *str, ssize_t bytes);


  private:
	std::string			_requestMethod;
  	std::string			_requestUri;
	unsigned int		_versionMinor;
	unsigned int		_versionMajor;
	request_headers 	_headers;
	request_body		_body;
	int					_parserState;
	int					_parserResult;
};