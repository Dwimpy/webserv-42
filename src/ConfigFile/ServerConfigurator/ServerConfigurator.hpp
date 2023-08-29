#pragma once
#include "ConfigFile.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include <vector>

class ServerConfigurator{
  public:
	ServerConfigurator(const std::string &confFilePath);
	~ServerConfigurator();
	void	addEmptyConfiguration();
	void	addEmptyServerBlock();
	void	addServerDirective();
	void	addLocationBlock();
	void	addLocationDirective();
	void 	pushToServerDirectiveKey(char c);
	void 	pushToServerDirectiveValue(char c);
	void	pushToLocationDirectiveKey(char c);
	void	pushToLocationDirectiveValue(char c);
	void	pushToLocationPath(char c);
  private:
	std::string					_input;
	std::vector<ServerBlock>	_serverBlocks;
	std::vector<ConfigFile>		_serverConfigs;

	void	createInputString(const std::string &confFilePath);
};