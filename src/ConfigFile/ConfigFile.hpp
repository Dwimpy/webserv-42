#pragma once
#include "ConfigFileParser.hpp"
#include "Directive.hpp"
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

	int 				getPort();
	int 				getPort() const;
	const std::string	&getServerName() const;
	bool 				isAllowedMethodServer(const std::string &method);
	bool				isValidLocation(const std::string &location);

  private:
	serverDirectives	_serverDirectives;
	locationDirectives	_locationDirectives;
};