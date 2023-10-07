#include "HttpResponse.hpp"
#include "ErrorResponse.hpp"
#include "GetResponse.hpp"
#include "DeleteResponse.hpp"
#include <fstream>
#include <string>
#include "EventHandler.hpp"
#include <sys/stat.h>

HttpResponse::HttpResponse(const HttpRequest &request, const ConfigFile &config): _statusCode(200), _statusError("OK")
{
//	config.inspectConfig();
	if (checkFileExists(request, config) && checkAllowedMethod(request, config))
	{
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

bool isDirectory(const char* path) {
	struct stat fileStat;
	if (stat(path, &fileStat) == 0) {
		return S_ISDIR(fileStat.st_mode);
	}
	return false;  // Error or file doesn't exist
}

bool HttpResponse::checkFileExists(const HttpRequest &request, const ConfigFile &config)
{
	std::string		path;
	std::ifstream	iss;
	bool			is_good = false;

	path = config.getFilePath(request);
	std::cout << "path " << path << std::endl;
	if (isDirectory(path.c_str()))
	{
		if (!config.checkAutoIndex(request))
			_statusCode = 404;
		else
			_statusCode = 469;
		std::cerr << "status code "<< _statusCode << std::endl;
		return (is_good);
	}
	iss.open(path);

	if (iss.good())
		is_good = true;
	else if (config.checkCgi(request.getRequestUri()))
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
		_statusCode = 404;
	iss.close();
//	std::cout << "file is :" << is_good << " path : " << path << std::endl;
	return (is_good);
}


bool HttpResponse::checkAllowedMethod(const HttpRequest &request, const ConfigFile &config)
{
	if (!config.isAllowedMethodServer(request.getRequestMethod()))
	{
		_statusCode = 405;
		return (false);
	}
	return (true);
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

