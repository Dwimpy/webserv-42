#include "ServerConfig.hpp"
#include "Server.hpp"

// Default constructors

ServerConfig::ServerConfig()
{
	setPort("8080");
	setHostName("Random server");
	setHost("127.0.0.1");
	setDocumentRoot("docs");
	setIndex("index.html");
}

ServerConfig::ServerConfig(const std::string &filename)
{

}

ServerConfig &ServerConfig::operator=(const ServerConfig &copy)
{
	if (this != &copy)
	{
		this->_host = copy._host;
		this->_port = copy._port;
		this->_index = copy._index;
		this->_hostname = copy._hostname;
		this->_documentRoot = copy._documentRoot;
	}
	return (*this);
}

ServerConfig::~ServerConfig() { }

// Setters
void	ServerConfig::setPort(const std::string &port)
{
	this->_port = port;
}

void	ServerConfig::setHostName(const std::string& hostname)
{
	this->_hostname = hostname;
}

void	ServerConfig::setHost(const std::string &host)
{
	this->_host = host;
}

void	ServerConfig::setDocumentRoot(const std::string &root)
{
	this->_documentRoot = root;
}

void	ServerConfig::setIndex(const std::string &index)
{
	this->_index = index;
}

//	Getters

const std::string &ServerConfig::getPort() const
{
	return (this->_port);
}


const std::string &ServerConfig::getHostName() const
{
	return (this->_hostname);
}

const std::string &ServerConfig::getHost() const
{
	return (this->_host);
}

const std::string &ServerConfig::getDocumentRoot() const
{
	return (this->_documentRoot);
}
const std::string &ServerConfig::getIndex() const
{
	return (this->_index);
}
