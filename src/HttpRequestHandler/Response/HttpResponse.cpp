#include "HttpResponse.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>
std::vector<std::string> splitStringByDot(const std::string& input) {
	std::vector<std::string> tokens;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, '.')) {
		tokens.push_back(token);
	}

	return tokens;
}

HttpResponse::HttpResponse(const HttpRequest &request, const ServerConfig &config): _statusCode(200), _statusError("OK")
{
	fileExists(request, config);

	appendResponseHeader(request);
    if (request.getValueByKey("Cookie") == "")
        appendCookie(request);
	else
        appendNewLine(request);
//		deleteCookie(request);

    std::string uri = request.getRequestUri();
	std::vector<std::string> result = splitStringByDot(uri);
    if (result.back() == "rs" || result.back() == "py")
    {
      	if (result.back() == "py")
            _flag = 1;
		else
			_flag = 0;

        if(pipe(_response_fd) == -1)
            std::cerr << ("pipe creation failed!") << std::endl;


        switch (fork())
        {
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

HttpResponse::HttpResponse(const HttpRequest &request, ConfigFile &config): _statusCode(200), _statusError("OK")
{
//	processRequestUri(request, config);
// TODO: Check location before actually checking the methods allowed
	if (!checkAllowedMethod(request, config))
	{
		_statusError = "KO";
		_statusCode = 504;
		_errorMessage = "Unauthorized method";
// TODO: Append error page
//		appendErrorPage(_statusCode);
	}
//	config.inspectConfig();
}

void HttpResponse::processRequestUri(const HttpRequest &request, ConfigFile &config)
{
	size_t	first;
	size_t	last;
	std::string request_uri;
	std::string	location;

	request_uri = request.getRequestUri();
	first = request_uri.find_first_of("/");
	last = request_uri.find_last_of("/");
	if (first != last)
	{
		location = request_uri.substr(first, last);
		if (!config.isValidLocation(location))
		{
			_statusCode = 404;
			_statusError = "KO";
			_errorMessage = "Location not found";
			std::cout << "INVALID" << std::endl;
		}
	}
}

void HttpResponse::processMethod(const HttpRequest &request)
{
	if (request.getRequestMethod() == "GET")
		processGetRequest(request);
}

void HttpResponse::processGetRequest(const HttpRequest &request)
{

}

bool HttpResponse::checkAllowedMethod(const HttpRequest &request, ConfigFile &config)
{

	return (config.isAllowedMethodServer(request.getRequestMethod()));
}

void HttpResponse::processRequestHeader(const HttpRequest &request)
{

}

void HttpResponse::appendResponseHeader(const HttpRequest &request)
{
	appendHttpProtocol(request);
	appendStatusCode(request);
	appendContentType(request);
	if (request.getRequestMethod() == "GET")
		appendContentLength(request);
}

HttpResponse::HttpResponse()
{}

HttpResponse::~HttpResponse()
{}

void	HttpResponse::appendHttpProtocol(const HttpRequest &request)
{
	_response.append("HTTP/");
	_response.append(std::to_string(request.getVersionMajor()));
	_response.append(".");
	_response.append(std::to_string(request.getVersionMajor()));
	_response.append(" ");
}

void	HttpResponse::appendCookie(const HttpRequest &request)
{
	_response.append("Set-Cookie: ");
	_response.append(Client::generateCookieId(6));
	_response.append("=");
	_response.append(Client::generateCookieId(12));
	_response.append("\r\n\r\n");
}
void	HttpResponse::appendNewLine(const HttpRequest &request)
{
	_response.append("\r\n");
}

void	HttpResponse::deleteCookie(const HttpRequest &request)
{
	_response.append("Set-Cookie: ");
	_response.append(request.getValueByKey("Cookie"));
	_response.append("=; expires=Thu, 01 Jan 1970 00:00:00 UTC; path=/;");
	_response.append("\r\n\r\n");
}


void	HttpResponse::appendStatusCode(const HttpRequest &request)
{
	_response.append(std::to_string(_statusCode));
	_response.append(" ");
	_response.append(_statusError);
	_response.append("\r\n");
}

void	HttpResponse::appendContentType(const HttpRequest &request)
{
	_response.append("Content-Type: ");
	_response.append(getContentType(request));
	_response.append("\r\n");
}

void	HttpResponse::appendContentLength(const HttpRequest &request)
{
	long fileSize = 0;
	_response.append("Content-Length: ");
	struct stat fileInfo;
	if (stat(_fileName.c_str(), &fileInfo) == 0)
	{
		// Get the file size from fileInfo.st_size
		fileSize = static_cast<long>(fileInfo.st_size);
//		std::cout << fileSize << " file size" << std::endl;
	}

	std::ostringstream oss;
	oss << fileSize;
	std::string contentLength = oss.str();
	_response.append(contentLength);
	_response.append("\r\n");
}

void	HttpResponse::appendFileContents()
{
	std::ifstream	infile(_fileName, std::ios::binary);
	infile.seekg(0, std::ios::end);
	std::string data;
	data.resize(infile.tellg());
	infile.seekg(0, std::ios::beg);
	infile.read(&data[0], data.size());
	infile.close();
	_response.append(data);
}

void	HttpResponse::fileExists(const HttpRequest &request, const ServerConfig &config)
{
	_fileName = config.getDocumentRoot() + getFileName(request);

	std::ifstream	infile(_fileName);
//	std::cout << _fileName << " " << request.getRequestUri() << std::endl;
	if (infile.good())
	{
		infile.close();
		return ;
	}
	else
	{
		this->_statusCode = 200;
		this->_statusError = "OK";
        this->_fileName = "./docs/error_pages/404.html";
	}
}

ssize_t	HttpResponse::getResponseSize()
{
	return (static_cast<ssize_t>(this->_response.size()));
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
	return (this->_response);
}


void    HttpResponse::createEnv(std::vector<std::string> &env, const HttpRequest &request)
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
	std::string var = request.getValueByKey("Cookie");
	env.push_back("Cookie=" + var);
	env.push_back("USERNAME=" + var.substr(0, 6));
	env.push_back("PWD=" + var.substr(7, 12));
	env.push_back("Error_code=" + std::to_string(this->_statusCode));
	env.push_back("Error_msg=" + this->_errorMessage);

//	std::cout << "cookie: " << var << std::endl;
//	std::cout << "username: " << var.substr(0, 6) << std::endl;
//	std::cout << "pwd: " << var.substr(7, 12) << std::endl;
//	env.push_back("Cookie=user:cookie" );
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


int HttpResponse::dup_request_to_stdin(const HttpRequest &request) {
    int fd[2];
    if (pipe(fd) == -1)
        return EXIT_FAILURE;
    std::string query;
    std::string body;
	body = request.getFullBody();
	std::string text(request.getRequestUri());
	size_t closingQuotePos = text.find(".py");
	if (closingQuotePos == std::string::npos)
    	query.append(body);
	else
		query.append(request.extractFileName(body));
    if (write(fd[STDOUT_FILENO], query.c_str(), query.length()) < 0)
    {
        close(fd[STDIN_FILENO]);
        close(fd[STDOUT_FILENO]);
        return EXIT_FAILURE;
    }
    close(fd[STDOUT_FILENO]);
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

    std::string cgiPath = getProjectDir()  + "/src/cgi/target/release/";

	if (_flag == 1)
	{
		cgiPath = getProjectDir() + "/src/cgi/src/upload.py";
	}
	else
	{
    	if (!cgiPath.empty() && chdir((cgiPath).c_str()) == -1)
		{
    	    error("404 CGI path not found!"); /* set 404 page */
		}
	}

	std::vector<std::string> result = splitStringByDot(request.getRequestUri());

	if (_flag != 1)
	{
        cgiPath += result.front();
	}

    createEnv(env, request);

	char *environment[env.size() + 1];
    for (size_t i = 0; i < env.size(); i++)
        environment[i] = const_cast<char *>(env[i].c_str());
    environment[env.size()] = nullptr;
    dup2(_response_fd[1], STDOUT_FILENO);
    close(_response_fd[1]);
    close(_response_fd[0]);

    if (dup_request_to_stdin(request))
        exit(error("tmpfile creation failed!"));

	char *args[2];
	args[0] = (char *)cgiPath.c_str();
	args[1] = nullptr;

    if (execve(cgiPath.c_str(), args,  environment) == -1)
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
			_statusError = "KO";
            return (EXIT_FAILURE);
        }
    }
    else if (WIFSIGNALED(status))
    {
        std::cerr << ("interrupted by signal!") << std::endl;
        return (EXIT_FAILURE);
    }
	_statusError = "OK";
    return EXIT_SUCCESS;
}

int HttpResponse::write_response()
{
    char			buffer[1000];
    long long 		bytes = 1;

//    lseek(_response_fd, 0, SEEK_SET);
	if (_statusError == "KO")
	{
		_fileName = "./docs/error_pages/404.html";
		appendFileContents();
		close(_response_fd[0]);
		return EXIT_SUCCESS;
	}
    while (bytes > 0) {
        bytes = read(_response_fd[0], buffer, sizeof(buffer));
        if (bytes < 0) {
            close(_response_fd[0]);
            return EXIT_FAILURE;
        }
        else
            _response.append(std::string(buffer, bytes));
    }
    close(_response_fd[0]);
    return	EXIT_SUCCESS;
}
