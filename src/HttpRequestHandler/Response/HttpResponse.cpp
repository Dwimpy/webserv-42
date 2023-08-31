#include "HttpResponse.hpp"
#include <fstream>
#include <iostream>
#include <string>

void    createEnv(std::vector<std::string> &env, const HttpRequest &request)
{
    env.push_back("REQUEST_METHOD=POST" );
    env.push_back("QUERY_STRING=" );
    env.push_back("SCRIPT_NAME=html/cgi/src/register_landing_page.rs" );
    env.push_back("REQUEST_URI=html/cgi/src/register_landing_page.rs" );
    env.push_back("PATH_INFO=html/cgi/src/register_landing_page.rs" );
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

const std::string getProjectDir() {
    char buffer[PATH_MAX];

    if (getcwd(buffer, sizeof(buffer)) != nullptr)
        return std::string(buffer);
    else
        error("getcwd fails");
    return "";
}

void    HttpResponse::childProcess(const HttpRequest &request)
{
    std::vector<std::string> env;
    std::string cgiScript;
    char *environment[env.size() + 1];
    std::string cgiPath = getProjectDir()  + "/src/cgi/target/release/" ;

    if (!cgiPath.empty() && chdir((cgiPath).c_str()) == -1)
        error("404 CGI path not found!"); /* set 404 page */

    createEnv(env, request);

    for (size_t i = 0; i < env.size(); i++)
        environment[i] = const_cast<char*>(env[i].c_str());
    environment[env.size()] = nullptr;

    if (_flag == 1)
        cgiPath += "register_landing_page";
    else
        cgiPath += "register";

    std::cout << "PATHHHHHH: " << cgiPath << std::endl;
    dup2(_response_fd[1], STDOUT_FILENO);
    close(_response_fd[1]);
    close(_response_fd[0]);
    if (dup_request_to_stdin())
        exit(error("tmpfile creation failed!"));

    if (execl(cgiPath.c_str(), nullptr, environment) == -1)
        exit(error("execve failed!"));
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
    if (request.getRequestUri() == "/register_landing_page.rs" || request.getRequestUri() == "/register.rs")
    {
        _flag = 0;
        if (request.getRequestUri() == "/register_landing_page.rs")
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