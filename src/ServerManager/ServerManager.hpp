#include <vector>
#include "Server.hpp"
#include "ServerConfigurator.hpp"
#include "SocketHandler.hpp"

typedef std::map<int, std::map<int, t_pollfd *> > indexToPollMap;

class ServerManager{

  public:
	ServerManager();
	~ServerManager();
	void	buildServers(const std::string &filePath);
	bool	startServers();
	void	runServers();
	void 	pollIncomingConnections();


  private:
	int							_pollReady;
	std::vector<Server>			_serverList;
	std::vector<t_pollfd>		_activeSockets;
	ServerConfigurator			_serverConfigurator;
	indexToPollMap				_serverToPollMap;

	void	parseConfigurationFile(const std::string &filePath);
	void	createAndConfigureServers();
	void	removeFd(std::vector<t_pollfd> &fds);
};

void	signalHandler(int signal);
