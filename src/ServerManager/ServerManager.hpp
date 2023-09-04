#include <vector>
#include "Server.hpp"
#include "ServerConfigurator.hpp"

class ServerManager{

  public:


	ServerManager();
	~ServerManager();
	void	buildServers(const std::string &filePath);
	bool	startServers();
	void	runServers();

	int		getNameWidth();

	static int findServerFromFd(std::vector<Server> &serverList, Client &client, int fd);

  private:
	std::vector<Server>			_serverList;
	ServerConfigurator			_serverConfigurator;

	void	parseConfigurationFile(const std::string &filePath);
	void	createAndConfigureServers();
};

void	signalHandler(int signal);
