#include "ErrorResponse.hpp"
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>
#include <dirent.h>
#include <stack>

ErrorResponse::ErrorResponse(int16_t statusCode, const HttpRequest &request, const ConfigFile &config): BaseResponse(request, config) {
		setStatusCode(statusCode);
		setContentType();
		if(_status_code == 469)
		{
			generateDirectoryListingPage();
			setStatusCode(200);
		}
		else if (config.checkCgi("/error.rs"))
			getContent("/error.rs");
		else
			stupidErrorPage();
		_contentType = "text/html";
}

void	ErrorResponse::stupidErrorPage()
{
	_content = "<html lang=\"en\">\n"
				   "<link rel=\"stylesheet\" href=\"/css/error.css\">\n"
				   "<script src=\"/js/stars.js\"></script>\n"
				   "<div class=\"text\">\n"
				   "    <div>ERROR</div>\n"
				   "    <h1>469</h1>\n"
				   "    <hr>\n"
				   "    <div>CGI not built</div>\n"
				   "</div>\n"
				   "<a href=\"../index.html\" class=\"button\">Go home</a>\n"
				   "<div class=\"astronaut\">\n"
				   "    <img src=\"../assets/astronaut.png\" alt=\"\" class=\"src\">\n"
				   "</div>\n"
				   "</html>";
}

void ErrorResponse::listDirectory(const char* path) {
		std::stack<std::string> dirStack;
		dirStack.push(path);
		while (!dirStack.empty()) {
			const std::string currentDir = dirStack.top();
			dirStack.pop();
			DIR* dir = opendir(currentDir.c_str());
			if (!dir) {
				std::cerr << "Error opening directory " << currentDir << ": " << strerror(errno) << std::endl;
				continue;
			}
			dirent* entry;
			while ((entry = readdir(dir)) != NULL) {
				if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
					_content += std::string(currentDir) + "/" + entry->d_name + "<br>";
					if (entry->d_type == DT_DIR) {
						std::string nextDir = currentDir + "/" + entry->d_name;
						dirStack.push(nextDir);
					}
				}
			}
			closedir(dir);
		}
}

void	ErrorResponse::generateDirectoryListingPage()
{
		listDirectory(_config.getFilePath(_request).c_str());
}

ErrorResponse::~ErrorResponse() {

}
