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
		else
			getContent("/error.rs");
		_contentType = "text/html";
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
