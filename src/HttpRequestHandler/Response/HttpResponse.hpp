#pragma once
#include <sstream>
#include <fstream>
#include "Server.hpp"
#include "ServerConfig.hpp"
#include "HttpRequest.hpp"

class HttpResponse{

  public:
	HttpResponse(const HttpRequest &request, const ServerConfig &config);
	HttpResponse(const HttpRequest &request, ConfigFile &config);
	HttpResponse();
	~HttpResponse();

	void    			createEnv(std::vector<std::string> &env, const HttpRequest &request);

	void				processRequestHeader(const HttpRequest &request);
	void				appendResponseHeader(const HttpRequest &request);
	bool 				checkAllowedMethod(const HttpRequest &request, ConfigFile &config);

	void				processMethod(const HttpRequest &request);
	void				processRequestUri(const HttpRequest &request, ConfigFile &config);
	void 				processGetRequest(const HttpRequest & request);
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

  private:
	unsigned int		_statusCode;
	std::string			_statusError;
	std::string			_errorMessage;
	std::string			_fileName;
	std::string			_response;
//    int                 _response_fd;
	int					_flag;
    int                 _response_fd[2];


	void				appendHttpProtocol(const HttpRequest &request);
	void				appendStatusCode(const HttpRequest &request);
	void				appendContentType(const HttpRequest &request);
};

int    error(std::string error);