#pragma once
#include "ServerBlock.hpp"
#include <vector>

class ConfigFile;

class ServerConfigurator{
  public:
	ServerConfigurator();
	~ServerConfigurator();

	void	parseConfigFile(const std::string &confFilePath);
	void	addEmptyServerBlock();
	void	addServerDirective();
	void	addLocationBlock();
	void	addLocationDirective();
	void 	pushToServerDirectiveKey(char c);
	void 	pushToServerDirectiveValue(char c);
	void	pushToLocationDirectiveKey(char c);
	void	pushToLocationDirectiveValue(char c);
	void	pushToLocationPath(char c);
	std::vector<ConfigFile>	buildConfigFiles();

  private:
	std::string					_input;
	std::vector<ServerBlock>	_serverBlocks;

	void	createInputString(const std::string &confFilePath);
};