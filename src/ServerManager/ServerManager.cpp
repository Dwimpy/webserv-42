#include "ServerManager.hpp"
#include "EventHandler.hpp"

const std::string GREEN = "\033[32m";
const std::string RED = "\033[31m";
const std::string RESET = "\033[0m";

static bool	shouldExit = false;

ServerManager::ServerManager(): _serverConfigurator(ServerConfigurator())
{
}

ServerManager::~ServerManager()
{}

void	ServerManager::buildServers(const std::string &filePath)
{
	parseConfigurationFile(filePath);
	createAndConfigureServers();
}

void ServerManager::runServers()
{
	EventHandler	event_handler;

	event_handler.initialize();
	event_handler.registerServers(_serverList);
	event_handler.eventLoop(_serverList);
}

bool ServerManager::startServers()
{

	for (size_t i = 0; i < _serverList.size(); ++i)
	{
		if (!_serverList[i].startServer())
			return (false);
		std::cout << "Server "
				  << "[ " << GREEN << std::setw(getNameWidth()) << std::left << _serverList[i].getConfiguration().getServerName() << RESET
				  << " ]" << " started:\t" << "Listening on port: " << "[ "
				  << GREEN  << std::setw(6) << _serverList[i].getConfiguration().getPort() << RESET << " ]" << std::endl;
	}
	return (true);
}

int	ServerManager::getNameWidth()
{
	int	max;
	int	len;

	max = 0;
	for (ssize_t i = 0; i < _serverList.size(); ++i)
	{
		len = _serverList[i].getConfiguration().getServerName().length();
		if (len > max)
			max = len;
	}
	return (max);
}

void	ServerManager::parseConfigurationFile(const std::string &filePath)
{
	_serverConfigurator.parseConfigFile(filePath);
}

void	ServerManager::createAndConfigureServers()
{
	std::vector<ConfigFile>	configs;
	configs = _serverConfigurator.buildConfigFiles();

	for (size_t i = 0; i < configs.size(); ++i)
		_serverList.push_back(configs[i]);
}

int ServerManager::findServerFromFd(std::vector<Server> &serverList, size_t &client, int fd)
{
	for (ssize_t k = 0; k < serverList.size(); ++k)
	{
		for (ssize_t j = 0; j < serverList[k].getConnectedClients().size(); ++j)
		{
			if (serverList[k].getConnectedClients()[j].getClientSocket().getFd() == fd)
			{
				client = j;
				return (k);
			}
		}
	}
	return (-2);
}
