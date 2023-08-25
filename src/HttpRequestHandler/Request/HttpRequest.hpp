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

		std::string getRequestMethod();
  private:
	std::string			_requestMethod;
  	std::string			_requestUri;
	unsigned int		_versionMinor;
	unsigned int		_versionMajor;
	request_headers 	_headers;
	request_body		_body;
};