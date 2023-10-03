#include "BaseResponse.hpp"

#include <fstream>
#include <sstream>
#include <unistd.h>
#include <chrono>
#include <random>
BaseResponse::BaseResponse(const HttpRequest &request, const ConfigFile &config): _request(request), _config(config), _status_code(200)
{
	_content = "";
	_contentType = "*/*";
}

BaseResponse::~BaseResponse() {}

void BaseResponse::setStatusCode(int16_t status_code)
{
	this->_status_code = status_code;
}

void BaseResponse::addHeader(const std::string &key, const std::string &value)
{
	_headers.insert(key, value);
}

std::string BaseResponse::getStatusCodeMessage() const {

	switch (_status_code)
	{
		case 100: return ("Continue");
		case 101: return ("Switching Protocols");
		case 200: return ("OK");
		case 201: return ("Created");
		case 202: return ("Accepted");
		case 204: return ("No Content");
		case 206: return ("Partial Content");
		case 300: return ("Multiple Choices");
		case 301: return ("Moved Permanently");
		case 302: return ("Found");
		case 303: return ("See Other");
		case 304: return ("Not Modified");
		case 307: return ("Temporary Redirect");
		case 308: return ("Permanent Redirect");
		case 400: return ("Bad Request");
		case 401: return ("Unauthorized");
		case 402: return ("Payment required");
		case 403: return ("Forbidden");
		case 404: return ("Not Found");
		case 405: return ("Method Not allowed");
		case 406: return ("Not Acceptable");
		case 407: return ("Proxy Authentication Required");
		case 408: return ("Request Timeout");
		case 409: return ("Conflict");
		case 410: return ("Gone");
		case 411: return ("Length Required");
		case 412: return ("Precondition Failed");
		case 413: return ("Payload Too large");
		case 414: return ("URI Too long");
		case 415: return ("Unsupported Media Type");
		case 416: return ("Range Not Satisfiable");
		case 417: return ("Expectation Failed");
		case 418: return ("I'm a teapot");
		case 422: return ("Unprocessable Entity");
		case 429: return ("Too Many Requests");
		case 431: return ("Request Header Fields Too Large");
		case 451: return ("Unavailable For Legal Reasons");
		case 500: return ("Internal Server Error");
		case 501: return ("Not Implemented");
		case 502: return ("Bad Gateway");
		case 503: return ("Service Unavailable");
		case 504: return ("Gateway Timeout");
		case 505: return ("HTTP Version Not Supported");
		case 506: return ("Variant Also Negotiates");
		case 507: return ("Insufficient Storage");
		case 508: return ("Loop Detected");
		case 510: return ("Not Extended");
		case 511: return ("Network Authentication required");
		default: return ("Unknown");
	}
}

std::vector<std::string> splitStringByDot(const std::string& input, char delim) {
	std::vector<std::string> tokens;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, delim)) {
		tokens.push_back(token);
	}
	return tokens;
}

void	BaseResponse::setContentType()
{
	std::string uri;
	std::string	extension;
	std::vector<std::string> result;
	uri = _config.getFilePath(_request);
	result = splitStringByDot(uri, '.');
	extension = result.back();

	std::string accept = _request.getValueByKey("Accept");
	ssize_t idx = accept.find(extension);
	if (idx != std::string::npos)
	{
		ssize_t startIdxl = accept.rfind(",", idx);
		if (startIdxl != std::string::npos)
			_contentType = accept.substr(startIdxl + 1, idx + extension.length() - startIdxl - 1);
		else
			_contentType = accept.substr(0, idx + extension.length());
	}
	else if (extension == "rs" || extension == "py")
		_contentType = "text/html";
}

std::string	BaseResponse::generateCookieId(int length)
{
	const std::string chars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	std::random_device					rd;
	std::mt19937						generator(rd());
	std::uniform_int_distribution<int>	distribution(0, chars.length() - 1);
	std::string                         sessionId;

	for (int i =0; i < length; ++i)
		sessionId += chars[distribution(generator)];
	return(sessionId);
}

void	BaseResponse::appendCookie(std::string &response)
{
	response.append("Set-Cookie: ");
	response.append(BaseResponse::generateCookieId(6));
	response.append("=");
	response.append(BaseResponse::generateCookieId(12));
	response.append("\r\n");
}

std::string BaseResponse::build()
{
	std::string	response;

	response += "HTTP/" +
				std::to_string(_request.getVersionMajor()) +
				"." +
				std::to_string(_request.getVersionMinor()) +
				" " +
				std::to_string(_status_code) +
				" " + getStatusCodeMessage() + "\r\n";

	OrderedMap<std::string, std::string>::iterator it;

	for (it = _headers.begin(); it != _headers.end(); ++it) {
		response += it->first + ": " + it->second + "\r\n";
	}

		response += "Content-type: " + _contentType + "\r\n";
		response += "Content-length: " + std::to_string(_content.length()) + "\r\n";
		if (_request.getValueByKey("Cookie") == "")
		{
			std::cout << "no cookies found" << std::endl;
			appendCookie(response);
		}
		response += "\r\n";

	if (_content.empty())
		return (response);
	return (response + _content + "\r\n");
}
void BaseResponse::createEnv(std::vector<std::string> &env)
{
	std::string var = _request.getValueByKey("Cookie");
	env.push_back("Cookie=" + var);
	env.push_back("Error_code=" + std::to_string(_status_code));
	env.push_back("Error_msg=" + getStatusCodeMessage());
}

int BaseResponse::dup_request_to_stdin() {
	    int fd[2];
    if (pipe(fd) == -1)
        return EXIT_FAILURE;

	std::string query;
    std::string body;
	body = _request.getFullBody();
//	std::string text(_config.getFilePath(_request));
//	size_t closingQuotePos = text.find(".py");
//	if (closingQuotePos == std::string::npos)

	if (_request.getValueByKey("Content-Type").find("multipart/form-data") != std::string::npos)
		query.append(_request.extractFileName(body));
	else
    	query.append(body);

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
void BaseResponse::childProcess(std::string const &uri) {
	std::vector<std::string> env;
    std::string cgiScript;
	std::string ext;
	ssize_t idx = _request.getRequestUri().rfind('.');

	if (idx != std::string::npos)
		ext = uri.substr(idx + 1, uri.size() - idx);
    std::string cgiPath = getProjectDir()  + _config.getCgiBin(_request, ext);

//	&& chdir((cgiPath).c_str()) == -1

    if (cgiPath.empty())
	{
		error("404 CGI path not found!"); /* set 404 page */
	}

	std::vector<std::string> result = splitStringByDot(uri, '.');

	if (result.back() == "rs")
        cgiPath += result.front();
	else
		cgiPath += uri;

    createEnv(env);

	char *environment[env.size() + 1];
    for (size_t i = 0; i < env.size(); i++)
        environment[i] = const_cast<char *>(env[i].c_str());
    environment[env.size()] = nullptr;

    dup2(_response_fd[1], STDOUT_FILENO);
    close(_response_fd[1]);
    close(_response_fd[0]);

    if (dup_request_to_stdin())
        exit(error("tmpfile creation failed!"));

	char *args[2];
	args[0] = (char *)cgiPath.c_str();
	args[1] = nullptr;
//	std::cerr << "cgi path " << cgiPath << std::endl;
    if (execve(cgiPath.c_str(), args,  environment) == -1)
        exit(error("execve failed!"));
}

void	BaseResponse::appendFileContents(const std::string &filename)
{
	std::ifstream	infile(filename, std::ios::binary);
	if (!infile.good())
	{
		return;
	}
	infile.seekg(0, std::ios::end);
	std::string data;
	data.resize(infile.tellg());
	infile.seekg(0, std::ios::beg);
	infile.read(&data[0], data.size());
	infile.close();
	_content.append(data);
}

void BaseResponse::getContent(const std::string &uri) {
	std::vector<std::string> result = splitStringByDot(uri, '.');
    if (result.back() == "rs" || result.back() == "py")
    {

        if(pipe(_response_fd) == -1)
            std::cerr << ("pipe creation failed!") << std::endl;


        switch (fork())
        {
            case -1:
                std::cerr << ("fork creation failed!") << std::endl;
                break;
            case 0:
                childProcess(uri);
        }

        if (parent_process() != EXIT_SUCCESS){
            error("EXITED PARENT");
        }
        if (write_response() != EXIT_SUCCESS)
        {
            std::cerr << ("write failed !") << std::endl;
            error("interrupted by signal!");
        }
    }
    else
        appendFileContents(_config.getFilePath(_request));
}


int BaseResponse::parent_process() {
    int status;

    waitpid(-1, &status, 0);
    close(_response_fd[1]);
    if (WIFEXITED(status)) {
        if (WEXITSTATUS(status))
        {
            std::cerr << ("execve failed!") << std::endl;
			_status_code = 404;
            return (EXIT_FAILURE);
        }
    }
    else if (WIFSIGNALED(status))
    {
        std::cerr << ("interrupted by signal!") << std::endl;
        return (EXIT_FAILURE);
    }
	_status_code = 200;
    return EXIT_SUCCESS;
}
int BaseResponse::write_response() {
    char			buffer[8192];
    long long 		bytes = 1;

//    lseek(_response_fd, 0, SEEK_SET);
	if (_status_code > 400)
	{
		std::string _fileName = _config.getErrorPage(_request);
		appendFileContents(_fileName);
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
            _content.append(std::string(buffer, bytes));
    }
    close(_response_fd[0]);
    return	EXIT_SUCCESS;
}

int error(std::string error)
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
