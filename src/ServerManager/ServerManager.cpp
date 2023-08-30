#include "ServerManager.hpp"
#include "Server.hpp"

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

void ServerManager::pollIncomingConnections()
{
	try {
		this->_pollReady = poll(_activeSockets.data(), _activeSockets.size(), -1);
		if ((this->_pollReady < 0 && errno != EINTR) || this->_pollReady == 0)
			throw std::runtime_error("Polling encountered an error. Exiting");
	} catch (std::exception &e){
		std::cerr << "Caught exception: " << e.what() << std::flush;
		exit(1);
	}
}

void ServerManager::runServers()
{
	int		pollingStatus = -1;

	while (!shouldExit)
	{
		pollIncomingConnections();
		for (size_t i = 0; i < _activeSockets.size(); ++i)
		{
			if (_activeSockets[i].revents == 0)
				continue ;
			if (_serverList[0].getSocket().getSocketFD() == _activeSockets[i].fd)
				_serverList[0].acceptIncomingConnections(_activeSockets, _serverToPollMap);
			else
			{
				_serverList[0].handleIncomingRequests(_serverToPollMap);
				_serverList[0].closeRemainingSockets();
			}
		}
	}
}

bool ServerManager::startServers()
{
	if (!_serverList[0].startServer())
		return (false);
	std::cout << "Server Started:\n   Listening on port: " << _serverList[0].getConfiguration().getPort() << std::endl;
	_activeSockets.push_back((t_pollfd){_serverList[0].getSocket().getSocketFD(), POLLIN, 0});
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

void	signalHandler(int signal)
{
	if (signal == SIGINT)
		shouldExit = true;
}
