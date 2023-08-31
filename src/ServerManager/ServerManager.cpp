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
	while (!shouldExit)
	{
		pollIncomingConnections();
		for (int i = 0; i < _serverList.size(); ++i)
		{
			if (_activeSockets[i].revents == 0)
				continue ;
			if (_serverList[i].getSocket().getSocketFD() == _activeSockets[i].fd)
				_serverList[i].acceptIncomingConnections(_activeSockets, _serverToPollMap);
		}
		for (int i = 0; i < _serverList.size(); ++i)
			_serverList[i].handleIncomingRequests(&_activeSockets[0], _serverToPollMap);
		removeFd(_activeSockets);
	}
}

void	ServerManager::removeFd(std::vector<t_pollfd> &fds)
{
	std::vector<t_pollfd >::const_iterator it = fds.cbegin();

	while (it != fds.cend())
	{
		if (it->fd == -1)
			it = fds.erase(it);
		else
			++it;
	}
}

bool ServerManager::startServers()
{
	for (size_t i = 0; i < _serverList.size(); ++i)
	{
		if (!_serverList[i].startServer())
			return (false);
		std::cout << "Server Started:\n   Listening on port: " << _serverList[i].getConfiguration().getPort() << std::endl;
		_activeSockets.push_back((t_pollfd){_serverList[i].getSocket().getSocketFD(), POLLIN, 0});
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

void	signalHandler(int signal)
{
	if (signal == SIGINT)
		shouldExit = true;
}
