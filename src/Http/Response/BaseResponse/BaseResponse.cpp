#include "BaseResponse.hpp"

#include <fstream>
#include <sstream>
#include <unistd.h>

BaseResponse::BaseResponse(const HttpRequest &request, const ConfigFile &config): _request(request), _config(config), _status_code(200)
{
	_content = "";
	setContentType();
	getContent();
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

std::string BaseResponse::getStatusCodeError() const {

	switch (_status_code)
	{
		case 200: return ("OK");
		case 404: return ("Page not found");
		case 405: return ("Method not allowed");
		default: return ("Unknown");
	}
}

std::vector<std::string> splitStringByDot(const std::string& input) {
	std::vector<std::string> tokens;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, '.')) {
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
	result = splitStringByDot(uri);
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
	else
		_contentType = "*/*";
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
				" " +
				getStatusCodeError() + "\r\n";

	OrderedMap<std::string, std::string>::iterator it;

	for (it = _headers.begin(); it != _headers.end(); ++it) {
		response += it->first + ": " + it->second + "\r\n";
	}

		response += "Content-type: " + _contentType + "\r\n";
		response += "Content-length: " + std::to_string(_content.length()) + "\r\n";
		response += "\r\n";

	if (_content.empty())
		return (response);
	return (response + _content + "\r\n");
}
void BaseResponse::createEnv(std::vector<std::string> &env)
{
	env.push_back("Error_code=" + std::to_string(_status_code));
	env.push_back("Error_msg=" + getStatusCodeError());
}

int BaseResponse::dup_request_to_stdin() {
	    int fd[2];
    if (pipe(fd) == -1)
        return EXIT_FAILURE;
    std::string query;
    std::string body;
	body = _request.getFullBody();
	std::string text(_config.getFilePath(_request));
	size_t closingQuotePos = text.find(".py");
	if (closingQuotePos == std::string::npos)
    	query.append(body);
	else
		query.append(_request.extractFileName(body));
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
void BaseResponse::childProcess() {
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

	std::vector<std::string> result = splitStringByDot(_request.getRequestUri());

	if (_flag != 1)
	{
        cgiPath += result.front();
	}

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

    if (execve(cgiPath.c_str(), args,  environment) == -1)
        exit(error("execve failed!"));
}

void	BaseResponse::appendFileContents(const std::string &filename)
{
	std::ifstream	infile(filename, std::ios::binary);
	if (!infile.good())
	{
		std::cerr << "file not found" << std::endl;
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

void BaseResponse::getContent() {
	std::string uri = _request.getRequestUri();
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
                childProcess();
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
	if (getStatusCodeError() == "Page not found")
	{
		std::string _fileName = "./docs/error_pages/404.html";
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
