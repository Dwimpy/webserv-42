#include "HttpResponse.hpp"
#include <fstream>
#include <iostream>
#include <string>

void    createEnv(std::vector<std::string> &env, const HttpRequest &request)
{
    env.push_back("REQUEST_METHOD=POST" );
    env.push_back("QUERY_STRING=" );
    env.push_back("SCRIPT_NAME=html/cgi/register_process.php" );
    env.push_back("REQUEST_URI=html/cgi/register_process.php" );
    env.push_back("PATH_INFO=html/cgi/register_process.php" );
    env.push_back("SERVER_PROTOCOL=HTTP/1.1");
    env.push_back("CONTENT_LENGTH=21");
    env.push_back("REMOTE_ADDR=" );
    env.push_back("HTTP_USER_AGENT=Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36" );
    env.push_back("RESPONSE_HEADER=HTTP/1.1 200 OK" );
    env.push_back("CONTENT_TYPE=application/x-www-form-urlencoded" );
//    env.push_back("$_POST[\"username\"] = \"johndoe\"");
//    env.push_back("$_POST[\"password\"] = \"secretpassword\"");
//    env.push_back("CONTENT_BODY=username=s&password=s");
//    env.push_back("username=s&password=s");

//    REQUEST_METHOD=GET
//    QUERY_STRING=
//    SCRIPT_NAME=html/cgi/maxwell.php
//    REQUEST_URI=html/cgi/maxwell.php
//    PATH_INFO=html/cgi/maxwell.php
//    SERVER_PROTOCOL=HTTP/1.1
//    CONTENT_LENGTH=0
//    REMOTE_ADDR=
//    HTTP_USER_AGENT=Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36
//    RESPONSE_HEADER=HTTP/1.1 200 OK
//    Content-Type: text/html
//    Content-Length:
//    CONTENT_TYPE=
//    HTTP_COOKIE=key=1170067506

//    if (_request.cookies)
//        _request.env.push_back("HTTP_COOKIE=key=" + toString<int>(_request.cookies));
//    else
//        _request.env.push_back("HTTP_COOKIE=");
}

int HttpResponse::dup_request_to_stdin() {
//    int fd = tmp_fd();
//    FILE*   tmp = tmpfile();
//    int fd = fileno(tmp);
    int fd[2];
    if (pipe(fd) == -1)
        return EXIT_FAILURE;
    std::string query;
    //cookie here
    query.append("username=s&password=s");
    if (write(fd[STDOUT_FILENO], query.c_str(), query.length()) < 0)
    {
        close(fd[STDIN_FILENO]);
        close(fd[STDOUT_FILENO]);
        return EXIT_FAILURE;
    }
//    std::cerr << "request query " << query.c_str() << std::endl;
    close(fd[STDOUT_FILENO]);
//    lseek(fd, 0, SEEK_SET);
    dup2(fd[STDIN_FILENO], STDIN_FILENO);
    close(fd[STDIN_FILENO]);
    return EXIT_SUCCESS;
}

int    error(std::string error)
{
    std::cerr << error << std::endl;
    return (EXIT_FAILURE);
}

void    HttpResponse::childProcess(const HttpRequest &request)
{
    std::vector<std::string> env;
    createEnv(env, request);

    char *environment[env.size() + 1];
    for (size_t i = 0; i < env.size(); i++)
        environment[i] = const_cast<char*>(env[i].c_str());
    environment[env.size()] = nullptr;

    char *arguments[3];
    arguments[0] = const_cast<char*>("/usr/bin/php");
    if(_flag == 1)
        arguments[1] = const_cast<char*>("/Users/dhendzel/Documents/webserv-42/docs/register_process.php");
    else
    {
        arguments[1] = const_cast<char*>("/Users/dhendzel/Documents/webserv-42/docs/register.php");
    }
    arguments[2] = NULL;

//    std::cerr << "argument 0 " << arguments[0] << "argument 1 " << arguments[1] << std::endl;
    dup2(_response_fd[1], STDOUT_FILENO);
    close(_response_fd[1]);
    close(_response_fd[0]);
    if(dup_request_to_stdin())
        error("tmpfile creation failed!");
    std::string cgi_path = "/Users/dhendzel/Documents/webserv-42/docs/";
    if (!cgi_path.empty() && chdir(cgi_path.c_str()) == -1)
        error("chdir failed!");
    if (execve(arguments[0], arguments, environment) == -1)
        error("execve failed!");
}

int	HttpResponse::parent_process() {
    int status;

    waitpid(-1, &status, 0);
    close(_response_fd[1]);
    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status))
        {
            std::cerr << ("execve failed!") << std::endl;
            return (EXIT_FAILURE);
        }
    }
    else if (WIFSIGNALED(status))
    {
        std::cerr << ("interrupted by signal!") << std::endl;
        return (EXIT_FAILURE);
    }
    return EXIT_SUCCESS;
}

int HttpResponse::write_response()
{
    char			buffer[1000];
    long long 		bytes = 1;

//    lseek(_response_fd, 0, SEEK_SET);
    while (bytes > 0) {
        bytes = read(_response_fd[0], buffer, sizeof(buffer));
        if (bytes < 0) {
            close(_response_fd[0]);
            return EXIT_FAILURE;
        }
        else
            _response << std::string(buffer, bytes);
    }
    close(_response_fd[0]);
    return	EXIT_SUCCESS;
}

HttpResponse::HttpResponse(const HttpRequest &request, const ServerConfig &config): _statusCode(200), _statusError("OK")
{
	fileExists(request, config);
	appendHttpProtocol(request);
	appendStatusCode(request);
	appendContentType(request);
    if (request.getRequestUri() == "/register_process.php" || request.getRequestUri() == "/register.php")
    {
        _flag = 0;
        if (request.getRequestUri() == "/register_process.php")
            _flag = 1;
//        FILE*   tmp = tmpfile();
//        _response_fd = fileno(tmp);
        if(pipe(_response_fd) == -1)
            std::cerr << ("tmpfile creation failed!") << std::endl;
//        if (_response_fd < 0)
        switch (fork()) {
            case -1:
                std::cerr << ("fork creation failed!") << std::endl;
                break;
            case 0:
                childProcess(request);
        }
        if (parent_process() != EXIT_SUCCESS){
            error("EXITED PARENT");
        }
        if (write_response() != EXIT_SUCCESS)
        {
//            std::cerr << ("interrupted by signal!") << std::endl;
            error("interrupted by signal!");
        }
    }
    else
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