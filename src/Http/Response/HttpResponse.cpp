#include "HttpResponse.hpp"
#include "ErrorResponse.hpp"
#include "GetResponse.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

std::vector<std::string> splitStringByDott(const std::string& input) {
	std::vector<std::string> tokens;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, '.')) {
		tokens.push_back(token);
	}

	return tokens;
}

HttpResponse::HttpResponse(const HttpRequest &request, const ConfigFile &config): _statusCode(200), _statusError("OK")
{
//	config.inspectConfig();
	if (checkFileExists(request, config))
		checkAllowedMethod(request, config);
	if (_statusCode >= 400)
		_response = ErrorResponse(_statusCode, request, config).build();
	else
		_response = GetResponse(request, config).build();
}

bool HttpResponse::checkFileExists(const HttpRequest &request, const ConfigFile &config)
{
	std::string		path;
	std::ifstream	iss;
	bool			is_good;

	path = config.getFilePath(request);
	is_good = false;
	iss.open(path);
	if (iss.good())
	{
		is_good = true;
	}
	else if (config.checkCgi(request))
	{
		iss.close();
		ssize_t idx = path.rfind("/");
		path = config.getCgiPath(request) + path.substr(idx, path.size() - idx);
		std::cout << "ful path: " << path << std::endl;
		iss.open(path);
		if (iss.good())
			is_good = true;
		else
			_statusCode = 404;
	}
	else
	{
		_statusCode = 404;
	}
	iss.close();
//	std::cout << "file is :" << is_good << " path : " << path << std::endl;
	return (is_good);
}


void HttpResponse::checkAllowedMethod(const HttpRequest &request, const ConfigFile &config)
{
	if (!config.isAllowedMethodServer(request.getRequestMethod()))
		_statusCode = 405;
}

HttpResponse::HttpResponse()
{}

HttpResponse::~HttpResponse()
{}

ssize_t	HttpResponse::getResponseSize()
{
	return (static_cast<ssize_t>(this->_response.size()));
}


std::string HttpResponse::getResponse()
{
	return (this->_response);
}

