#include "ConfigFile.hpp"
#include <exception>

ConfigFile::ConfigFile()
{}

ConfigFile::~ConfigFile()
{}

ConfigFile::ConfigFile(const ServerBlock &serverBlock)
{
	size_t			dirVectorSize;
	size_t			locBlocksSize;
	Directive		dir;
	LocationBlock	loc;
	std::string		path;

	dirVectorSize = serverBlock.getServerDirectives().size();
	locBlocksSize = serverBlock.getLocationBlocks().size();
	for (size_t i = 0; i < dirVectorSize; ++i)
	{
		dir = serverBlock.getServerDirectives()[i];
		if (!dir.isValidDirective())
		{
			std::cerr << "Invalid directive provided: " << dir.getKey() << std::endl;
			exit(1);
		}
		addServerDirective(dir.getKey(), dir.getValue());
	}
	for (size_t i = 0; i < locBlocksSize; ++i)
	{
		loc = serverBlock.getLocationBlocks()[i];
		path = loc.getPath();
		for (size_t j = 0; j < loc.getDirectives().size(); ++j)
		{
			dir = loc.getDirectives()[j];
			if (!dir.isValidDirective())
			{
				std::cerr << "Invalid location directive provided: " << dir.getKey() << std::endl;
				exit(1);
			}
			addLocationDirective(path, dir.getKey(), dir.getValue());
		}
	}
}

void ConfigFile::addServerDirective(const std::string &key, const std::string &value)
{
	this->_serverDirectives[key] = value;
}

void ConfigFile::addLocationDirective(const std::string &path, const std::string &key, const std::string &value)
{
	this->_locationDirectives[path][key] = value;
}

int ConfigFile::getPort()
{
	try {
		return (std::stoi(_serverDirectives["listen"]));
	}catch (const std::exception &e) {
		throw ;
	}
}

int ConfigFile::getPort() const
{
	serverDirectives::const_iterator it = _serverDirectives.find("listen");

		try {
			return (std::stoi(it->second));
		}catch (const std::exception &e) {
			throw ;
		}
}


void ConfigFile::inspectConfig()
{
	std::cout << "Server Directives: " << "\n";

	for (serverDirectives::const_iterator it = _serverDirectives.begin(); it != _serverDirectives.end(); ++it)
	{
		std::cout << "Key: " << it->first << "\tValue: " << it->second << std::endl;
	}
	std::cout << "\nLocation Path and Directives: " << "\n";

	for (locationDirectives::const_iterator outerIt = _locationDirectives.begin(); outerIt != _locationDirectives.end(); ++outerIt)
	{
		std::cout << "Path: " << outerIt->first << "\n";
		for (serverDirectives::const_iterator innerIt = outerIt->second.begin(); innerIt != outerIt->second.end(); ++innerIt)
			 std::cout << "\tKey: " << innerIt->first << "\tValue: " << innerIt->second << std::endl;
	}
}

void ConfigFile::inspectConfig() const
{
	std::cout << "Server Directives: " << "\n";

	for (serverDirectives::const_iterator it = _serverDirectives.begin(); it != _serverDirectives.end(); ++it)
	{
		std::cout << "Key: " << it->first << "\tValue: " << it->second << std::endl;
	}
	std::cout << "\nLocation Path and Directives: " << "\n";

	for (locationDirectives::const_iterator outerIt = _locationDirectives.begin(); outerIt != _locationDirectives.end(); ++outerIt)
	{
		std::cout << "Path: " << outerIt->first << "\n";
		for (serverDirectives::const_iterator innerIt = outerIt->second.begin(); innerIt != outerIt->second.end(); ++innerIt)
			 std::cout << "\tKey: " << innerIt->first << "\tValue: " << innerIt->second << std::endl;
	}
}

 const std::string &ConfigFile::getServerName() const
{
	return (_serverDirectives.find("server_name")->second);
}

bool ConfigFile::isAllowedMethodServer(const std::string &method)
{
	if (_serverDirectives.find("allowed_methods") == _serverDirectives.end())
		return (method == "GET" || method == "POST" || method == "DELETE");
	else
	{
		if (_serverDirectives["allowed_methods"].find(method) != std::string::npos)
			return (true);
		return (false);
	}
}
bool ConfigFile::isValidLocation(const std::string &location)
{
	return (_locationDirectives.find(location) != _locationDirectives.end());
}
