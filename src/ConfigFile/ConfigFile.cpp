#include "ConfigFile.hpp"
#include "BaseResponse.hpp"
#include "HttpRequest.hpp"

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

bool ConfigFile::isAllowedMethodServer(const std::string &method) const
{
	if (_serverDirectives.find("allowed_methods") == _serverDirectives.end())
		return (method == "GET" || method == "POST" || method == "DELETE");
	else
	{
		if (_serverDirectives.find("allowed_methods")->second.find(method) != std::string::npos)
			return (true);
		return (false);
	}
}

std::string	ConfigFile::getAllowedMethods() const {
	if (_serverDirectives.find("allowed_methods") == _serverDirectives.end())
		return ("GET POST DELETE");
	return (this->_serverDirectives.find("allowed_methods")->second);
}

bool ConfigFile::isValidLocation(const std::string &location) const
{
	return (_locationDirectives.find(location) != _locationDirectives.end());
}

std::string ConfigFile::getServerRoot() const {

	serverDirectives::const_iterator it;

	it = _serverDirectives.find("root");
	if (it != _serverDirectives.end()) {
		return (it->second);
	} else {
		return ("./docs");
	}
}

std::string ConfigFile::getLocationPath(std::string location) const {
	locationDirectives::const_iterator it;
	std::string	biggest = "";
	for (it = _locationDirectives.cbegin(); it != _locationDirectives.cend(); ++it) {
		std::string curr = it->first;
		if (location.find(curr) != std::string::npos && curr.size() > biggest.size())
			biggest = curr;
	}
	return (biggest);
}

std::string ConfigFile::getFilePath(const HttpRequest &request) const {
	std::string location = getLocationPath(request.getRequestUri());
	std::string	location_root;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
	std::map<std::string, std::string> entry;
	std::map<std::string, std::string>::const_iterator map_it;
	std::string index_page = "index.html";

	if (location.empty())
		location = "/";
	if (location == request.getRequestUri())
	{
		it = _locationDirectives.find(location);
		if (it != _locationDirectives.cend())
		{
			map_it = it->second.find("index");
			if (map_it != it->second.cend())
				index_page = map_it->second;
		}
		return (getServerRoot() + location + index_page);
	}
	it = _locationDirectives.find(location);
	if (it != _locationDirectives.cend()) {
		entry = it->second;
		if (!entry.empty()) {
			if (entry.find("root") != entry.end()) {
				location = entry.find("root")->second;
				return (getServerRoot() + location + request.getRequestUri().substr(0, request.getRequestUri().size()));
			}
		}
	}
	return (getServerRoot() + location + request.getRequestUri().substr(location.size(), request.getRequestUri().size() - location.size()));
}

std::string ConfigFile::getErrorPage(const HttpRequest &request) const {
	std::string location = getLocationPath(request.getRequestUri());
	std::vector<std::string> tokens;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
	std::map<std::string, std::string> entry;
	std::map<std::string, std::string>::const_iterator map_it;
	std::string error_page = "error_pages/404.html";

	it = _locationDirectives.find(location);
	if (it != _locationDirectives.cend())
	{
		map_it = it->second.find("error_page");
		if (map_it != it->second.cend())
		{
			tokens = splitStringByDot(error_page, ' ');
			if (!error_page.empty())
				error_page = tokens[1];
		}
	}
	return (getServerRoot() + location + error_page);
}

bool	ConfigFile::checkCgi(const HttpRequest &request) const
{
	std::string location = getLocationPath(request.getRequestUri());
	std::vector<std::string> tokens;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
	std::map<std::string, std::string> entry;
	std::map<std::string, std::string>::const_iterator map_it;
	bool		allowed = false;

	if (location.empty())
		location = "/";
	it = _locationDirectives.find(location);
	if (it != _locationDirectives.cend())
	{
		map_it = it->second.find("cgi");
		if (map_it != it->second.cend())
		{
			ssize_t idx = request.getRequestUri().rfind('.');
			if (idx != std::string::npos)
			{
				std::string ext = request.getRequestUri().substr(idx + 1, request.getRequestUri().size() - idx);
				if(map_it->second.find(ext) != std::string::npos)
					allowed = true;
			}
		}
	}
	return (allowed);
}

bool	ConfigFile::checkCgiString(const std::string &uri) const
{
	std::string location = getLocationPath(uri);
	std::vector<std::string> tokens;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
	std::map<std::string, std::string> entry;
	std::map<std::string, std::string>::const_iterator map_it;
	bool		allowed = false;

	if (location.empty())
		location = "/";
	it = _locationDirectives.find(location);
	if (it != _locationDirectives.cend())
	{
		map_it = it->second.find("cgi");
		if (map_it != it->second.cend())
		{
			ssize_t idx = uri.rfind('.');
			if (idx != std::string::npos)
			{
				std::string ext = uri.substr(idx + 1, uri.size() - idx);
				if(map_it->second.find(ext) != std::string::npos)
					allowed = true;
			}
		}
	}
	return (allowed);
}

std::string ConfigFile::getCgiBin(const HttpRequest &request, std::string ext) const
{
	std::string location = getLocationPath(request.getRequestUri());
	std::vector<std::string> tokens;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
	std::map<std::string, std::string> entry;
	std::map<std::string, std::string>::const_iterator map_it;
	std::string	cgiBin = "src/cgi/target/release/";
	bool	notFound = true;

	it = _locationDirectives.find(location);
	if (it != _locationDirectives.cend())
	{
		map_it = it->second.find("cgi_bin");
		tokens = splitStringByDot(map_it->second, ' ');
		std::vector<std::string>::iterator it;
		it = std::find(tokens.begin(), tokens.end(), ext);
		if (it != tokens.end()) {
			cgiBin = tokens[std::distance(tokens.begin(), it) + 1];
			notFound = false;
		}
	}
	if (notFound)
	{
		map_it = _serverDirectives.find("cgi_bin");
		tokens = splitStringByDot(map_it->second, ' ');
		std::vector<std::string>::iterator it;
		it = std::find(tokens.begin(), tokens.end(), ext);
		if (it != tokens.end()) {
			cgiBin = tokens[std::distance(tokens.begin(), it) + 1];
		}
	}
	return (location + cgiBin);
}

std::string ConfigFile::getCgiPath(const HttpRequest &request) const
{
	std::string location = getLocationPath(request.getRequestUri());
	std::vector<std::string> tokens;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
	std::map<std::string, std::string> entry;
	std::map<std::string, std::string>::const_iterator map_it;
	std::string cgiPath = "src/cgi/src";
	bool	notFound = true;


	it = _locationDirectives.find(location);
	if (it != _locationDirectives.cend())
	{
		map_it = it->second.find("cgi_path");
		if (map_it != it->second.cend())
		{
			cgiPath = map_it->second;
			notFound = false;
		}
	}
	if (notFound)
	{
		if(_serverDirectives.find("cgi_path") != _serverDirectives.cend())
			cgiPath = _serverDirectives.find("cgi_path")->second;
//		std::cout << "PATH: " <<  cgiPath << std::endl;
		return (cgiPath);
	}
	return (location + cgiPath);
}

long ConfigFile::getMaxBodySize(const HttpRequest &request) const {
	std::string location = getLocationPath(request.getRequestUri());
	std::vector<std::string> tokens;
	std::map<std::string, std::map<std::string, std::string> >::const_iterator it;
	std::map<std::string, std::string> entry;
	std::map<std::string, std::string>::const_iterator map_it;
	long 	maxBodySize = 100000000;
	bool	notFound = true;


	it = _locationDirectives.find(location);
	if (it != _locationDirectives.cend())
	{
		map_it = it->second.find("client_max_body_size");
		if (map_it != it->second.cend())
		{
//			std::cerr << "max body 1 " << map_it->second << std::endl;
			maxBodySize = std::stol(map_it->second);
			notFound = false;
		}
	}
	if (notFound)
	{
		if(_serverDirectives.find("client_max_body_size") != _serverDirectives.cend())
		{
//			std::cerr << "max body 2 " << _serverDirectives.find("client_max_body_size")->second << std::endl;
			maxBodySize = std::stol(_serverDirectives.find("client_max_body_size")->second);
		}

	}
	return (maxBodySize);
}
