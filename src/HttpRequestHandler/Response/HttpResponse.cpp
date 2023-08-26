#include "HttpResponse.hpp"
#include <fstream>
#include <iostream>
#include <string>

HttpResponse::HttpResponse(const HttpRequest &request, const ServerConfig &config): _statusCode(200), _statusError("OK")
{
	fileExists(request, config);
	appendHttpProtocol(request);
	appendStatusCode(request);
	appendContentType(request);
    appendFileContents();
}

HttpResponse::HttpResponse()
{}

HttpResponse::~HttpResponse()
{}

void	HttpResponse::appendHttpProtocol(const HttpRequest &request)
{
	_response << "HTTP/" << request.getVersionMajor() << "." << request.getVersionMinor() << " ";
}

void	HttpResponse::appendStatusCode(const HttpRequest &request)
{
	_response << _statusCode << " " << _statusError << "\r\n";
}

void	HttpResponse::appendContentType(const HttpRequest &request)
{
	_response << "Content-Type: " << getContentType(request) << "\r\n\r\n";
}

void	HttpResponse::appendFileContents()
{
	std::ifstream	infile(_fileName);
	char			c;

	while (infile.get(c))
		_response << c;
	infile.close();
}

void	HttpResponse::fileExists(const HttpRequest &request, const ServerConfig &config)
{
	_fileName = config.getDocumentRoot() + getFileName(request);
	std::ifstream	infile(_fileName);

	if (infile.good())
	{
		infile.close();
		return ;
	}
	else
	{
		this->_statusCode = 404;
		this->_statusError = "KO";
        this->_fileName = "./docs/error_pages/404.html";
	}
}

std::string	HttpResponse::getContentType(const HttpRequest &request)
{
	size_t		idx;
	std::string	responseMsg;

	responseMsg = request.getValueByKey("Accept");
	idx = responseMsg.find_first_of(',');
	if (idx == std::string::npos)
		return (responseMsg);
	return (responseMsg.substr(0, idx));
}

std::string	HttpResponse::getFileName(const HttpRequest &request)
{
	if (request.getRequestUri() == "/")
		return ("/index.html");
	return (request.getRequestUri());
}

std::string HttpResponse::getResponse()
{
	return (this->_response.str());
}