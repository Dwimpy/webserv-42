#include "HttpResponse.hpp"
#include "ErrorResponse.hpp"
#include "GetResponse.hpp"
#include "DeleteResponse.hpp"
#include <fstream>
#include <string>
#include "EventHandler.hpp"

HttpResponse::HttpResponse(const HttpRequest &request, const ConfigFile &config): _statusCode(200), _statusError("OK")
{
//	config.inspectConfig();
	if (checkFileExists(request, config))
	{
		checkAllowedMethod(request, config);
		if(checkMaxBodySize(request, config))
		{
			if (request.getRequestMethod() == "POST" &&
			request.getValueByKey("Content-Type").find("multipart/form-data") != std::string::npos)
				uploadFile(request);
		}
	}
	if (_statusCode >= 400)
		_response = ErrorResponse(_statusCode, request, config).build();
	else if (request.getRequestMethod() == "DELETE")
		_response = DeleteResponse(request, config).build();
	else
		_response = GetResponse(request, config).build();
}

bool	HttpResponse::checkMaxBodySize(const HttpRequest &request, const ConfigFile &config)
{
	if (!request.getValueByKey("Content-Length").empty())
	{
		if (std::stol(request.getValueByKey("Content-Length")) >= config.getMaxBodySize(request))
		{
//			std::cerr << "_status code change " << std::endl;
			_statusCode = 413;
			return false;
		}
	}
	return true;
}

bool HttpResponse::checkFileExists(const HttpRequest &request, const ConfigFile &config)
{
	std::string		path;
	std::ifstream	iss;
	bool			is_good;

	path = config.getFilePath(request);
	is_good = false;
	iss.open(path);
		std::cerr << "ful path: " << path << std::endl;

	if (iss.good())
	{
		is_good = true;
	}
	else if (config.checkCgi(request))
	{
		iss.close();
		ssize_t idx = path.rfind('/');
		path = config.getCgiPath(request) + path.substr(idx, path.size() - idx);
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
	std::cout << "file is :" << is_good << " path : " << path << std::endl;
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

