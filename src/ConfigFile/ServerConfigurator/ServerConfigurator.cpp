#include "ServerConfigurator.hpp"
#include "ConfigFile.hpp"
#include "ConfigFileParser.hpp"
#include "Server.hpp"
#include "ServerBlock.hpp"
#include <fstream>
#include <iostream>

ServerConfigurator::ServerConfigurator(const std::string &confFilePath)
{
	const char	*cstr_input;

	createInputString(confFilePath);
	cstr_input = _input.c_str();
	ConfigFileParser::parseFile(*this, cstr_input, cstr_input + _input.length());
	if (ConfigFileParser::isParsingError())
		exit (1);
	for (size_t i = 0; i < _serverBlocks.size(); i++)
		_serverBlocks[i].printDirectives();
}

ServerConfigurator::~ServerConfigurator(){}

void ServerConfigurator::addEmptyConfiguration()
{
	this->_serverConfigs.__emplace_back(ConfigFile());
}

void ServerConfigurator::addEmptyServerBlock()
{
	this->_serverBlocks.__emplace_back(ServerBlock());
}

void ServerConfigurator::addServerDirective()
{
	this->_serverBlocks.back().addDirective();
}

void ServerConfigurator::pushToServerDirectiveKey(char c)
{
	this->_serverBlocks.back().pushToDirectiveKey(c);
}

void ServerConfigurator::pushToServerDirectiveValue(char c)
{
	this->_serverBlocks.back().pushToDirectiveValue(c);
}

void ServerConfigurator::pushToLocationDirectiveKey(char c)
{
	this->_serverBlocks.back().pushToLocationDirectiveKey(c);
}

void ServerConfigurator::pushToLocationDirectiveValue(char c)
{
	this->_serverBlocks.back().pushToLocationDirectiveValue(c);
}

void ServerConfigurator::pushToLocationPath(char c)
{
	this->_serverBlocks.back().pushToLocationPath(c);
}

void ServerConfigurator::addLocationBlock()
{
	this->_serverBlocks.back().addLocationBlock();
}

void ServerConfigurator::addLocationDirective()
{
	this->_serverBlocks.back().addLocationDirective();
}


void ServerConfigurator::createInputString(const std::string &confFilePath)
{
	std::ifstream input;
	std::string line;
	size_t		begin;
	size_t		end;
	std::string	trimmedLine;

	input.open(confFilePath);
	if (input.is_open())
	{
		while (getline(input, line))
		{
			begin = line.find_first_not_of(" \t");
			if (begin == std::string::npos)
				begin = 0;
			end = line.find_last_not_of(" \t\n");
			if (end == std::string::npos)
				end = line.length() - 1;
			trimmedLine = line.substr(begin, end + 1);
			if (trimmedLine[0] != '#')
				_input += trimmedLine + "\n";
		}
		input.close();
	}
	else
	{
		std::perror("Error opening configuration file");
		exit(1);
	}
}