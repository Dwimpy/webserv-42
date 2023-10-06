#pragma once
#include "ConfigFileParser.hpp"
#include "Directive.hpp"
#include "HttpRequest.hpp"

#include <iostream>

typedef std::map<std::string, std::string> serverDirectives;
typedef std::map<std::string, std::map<std::string, std::string> >	locationDirectives;

class ServerBlock;

class ConfigFile{

  public:
	ConfigFile();
	ConfigFile(const ServerBlock &serverBlock);
	~ConfigFile();
	void	addServerDirective(const std::string &key, const std::string &value);
	void	addLocationDirective(const std::string &path, const std::string &key, const std::string &value);
	void	inspectConfig();
	void	inspectConfig() const;
	std::string getAllowedMethods() const;
	std::string	getServerRoot() const;

	int 				getPort();
	int 				getPort() const;
	std::string			getErrorPage(const HttpRequest &request, int error_code) const;
	const std::string	&getServerName() const;
	bool 				isAllowedMethodServer(const std::string &method) const;
	bool				isValidLocation(const std::string &location) const;
	std::string			getLocationPath(std::string location) const;
	std::string			getFilePath(const HttpRequest &request) const;
	std::string 		getCgiPath(const HttpRequest &request) const;
	bool				checkCgi(const HttpRequest &request) const;
	bool				checkCgiString(const std::string &uri) const;
	std::string 		getCgiBin(const HttpRequest &request, std::string ext) const;
	long 				getMaxBodySize(const HttpRequest &request) const;
	bool				checkAutoIndex(const HttpRequest &request) const;

  private:
	serverDirectives	_serverDirectives;
	locationDirectives	_locationDirectives;
};