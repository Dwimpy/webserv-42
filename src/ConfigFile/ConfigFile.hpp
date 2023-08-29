#pragma once
#include "ServerBlock.hpp"

class ConfigFile{
  public:
	ConfigFile();
	~ConfigFile();
	void	addEmptyServerBlock();

  private:
	ServerBlock	_config;
};