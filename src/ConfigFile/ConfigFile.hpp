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
//	void	inspectConfig();
//	void	inspectConfig() const;
//	std::string getAllowedMethods() const;

	std::string getServerRoot(const std::string &uri) const;

	int 				getPort();
	int 				getPort() const;
//	std::string			getErrorPage(const std::string &uri, int error_code) const;
	const std::string	&getServerName() const;
	bool 				isAllowedMethodServer(const HttpRequest &request) const;
//	bool				isValidLocation(const std::string &location) const;
	std::string			getLocationPath(const std::string &location) const;
	std::string			getFilePath(const std::string &uri) const;
	std::string 		getCgiPath(const std::string &uri) const;
	bool				checkCgi(const std::string &uri, const std::string &ext) const;
	std::string 		getCgiBin(const std::string &uri, const std::string &ext) const;
	long 				getMaxBodySize(const std::string &uri) const;
	bool				checkAutoIndex(const std::string &uri) const;



  private:
	serverDirectives	_serverDirectives;
	locationDirectives	_locationDirectives;
};