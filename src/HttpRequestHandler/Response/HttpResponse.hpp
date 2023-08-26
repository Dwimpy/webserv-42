#pragma once
#include <sstream>
#include <fstream>
#include "Server.hpp"
#include "ServerConfig.hpp"
#include "HttpRequest.hpp"

class HttpResponse{

  public:
	HttpResponse(const HttpRequest &request, const ServerConfig &config);
	HttpResponse();
	~HttpResponse();
	void				appendHttpProtocol(const HttpRequest &request);
	void				appendStatusCode(const HttpRequest &request);
	void				appendContentType(const HttpRequest &request);
	void				appendFileContents();
	void				fileExists(const HttpRequest &request, const ServerConfig &config);
	std::string			getContentType(const HttpRequest &request);
	std::string			getFileName(const HttpRequest &request);
	std::string			getResponse();

  private:
	unsigned int		_statusCode;
	std::string			_statusError;
	std::string			_fileName;
	std::stringstream	_response;
};