#pragma once
#include "BaseResponse.hpp"
#include "HttpRequest.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"

#include <fstream>
#include <sstream>

class HttpResponse{

  public:
	HttpResponse(const HttpRequest &request, const ConfigFile &config);
	HttpResponse();
	~HttpResponse();

	void 				checkAllowedMethod(const HttpRequest &request, const ConfigFile &config);
	std::string			getResponse();


	ssize_t				getResponseSize();
	bool 				checkFileExists(const HttpRequest &request, const ConfigFile &config);

  private:
	int16_t				_statusCode;
	std::string			_statusError;
	std::string			_errorMessage;
	std::string			_fileName;
	std::string			_response;

};
