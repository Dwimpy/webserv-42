#pragma once
#include "BaseResponse.hpp"
#include "HttpRequest.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"

#include <fstream>
#include <sstream>

class HttpResponse{

  public:
	HttpResponse(const HttpRequest &request, const ServerConfig &config);
	HttpResponse(const HttpRequest &request, const ConfigFile &config);
	HttpResponse();
	~HttpResponse();

	void    			createEnv(std::vector<std::string> &env, const HttpRequest &request);

	void				appendResponseHeader(const HttpRequest &request);
	bool 				checkAllowedMethod(const HttpRequest &request, const ConfigFile &config);

	void				processRequestUri(const HttpRequest &request, ConfigFile &config);
    void	            appendCookie(const HttpRequest &request);
    void	            appendNewLine(const HttpRequest &request);
	void				appendFileContents();
	void				deleteCookie(const HttpRequest &request);
	void				fileExists(const HttpRequest &request, const ServerConfig &config);
	std::string			getContentType(const HttpRequest &request);
	std::string			getFileName(const HttpRequest &request);
	std::string			getResponse();

    int                 write_response();
    void                childProcess(const HttpRequest &request);
    int                 parent_process();
    int                 dup_request_to_stdin(const HttpRequest &request);
	ssize_t				getResponseSize();
	bool 				checkFileExists(const HttpRequest &request, const ConfigFile &config);

  private:
	int16_t				_statusCode;
	std::string			_statusError;
	std::string			_errorMessage;
	std::string			_fileName;
	std::string			_response;
//    int                 _response_fd;
	int					_flag;
    int                 _response_fd[2];
	BaseResponse		*_re;

	void				appendHttpProtocol(const HttpRequest &request);
	void				appendStatusCode(const HttpRequest &request);
	void				appendContentType(const HttpRequest &request);
	void				appendContentLength(const HttpRequest &request);
};

int errorr(std::string error);