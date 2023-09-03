#include "ServerManager.hpp"
#include "EventHandler.hpp"


static bool	shouldExit = false;

ServerManager::ServerManager(): _serverConfigurator(ServerConfigurator())
{
	signal(SIGINT, signalHandler);
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
		std::cout << "Server Started:\n   Listening on port: " << _serverList[i].getConfiguration().getPort() << std::endl;
	}
	return (true);
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
	{
		std::cout << "Server " << i + 1 << "\n";
		_serverList.__emplace_back(configs[i]);
		std::cout << "Port: " << _serverList.back().getConfiguration().getPort() << "\n";
		std::cout << "\n";
	}
}

int ServerManager::findServerFromFd(std::vector<Server> &serverList, int fd)
{
	for (ssize_t k = 0; k < serverList.size(); ++k)
	{
		for (ssize_t j = 0; j < serverList[k].getConnectedClients().size(); ++j)
		{
			if (serverList[k].getConnectedClients()[j].getClientSocket().getFd() == fd)
			{
				serverList[k].getConnectedClients()[j].setClientFd((-1));
				return (k);
			}
		}
	}
	return (-2);
}


void	signalHandler(int signal)
{
	if (signal == SIGINT)
		shouldExit = true;
}
