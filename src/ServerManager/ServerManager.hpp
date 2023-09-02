#include <vector>
#include "Server.hpp"
#include "ServerConfigurator.hpp"

typedef std::map<int, std::map<int, t_pollfd *> > indexToPollMap;

class ServerManager{

  public:
	ServerManager();
	~ServerManager();
	void	buildServers(const std::string &filePath);
	bool	startServers();
	void	runServers();
	void 	pollIncomingConnections();
	void	handleServerRequests(t_pollfd *pollfd);


  private:
	int							_pollReady;
	std::vector<Server>			_serverList;
	std::vector<t_pollfd>		_activeSockets;
	ServerConfigurator			_serverConfigurator;
	indexToPollMap				_serverToPollMap;
	std::map<t_pollfd *, int> 	_pollToServerMap;

	void	parseConfigurationFile(const std::string &filePath);
	void	createAndConfigureServers();
	void	removeFd(std::vector<t_pollfd> &fds);
};

void	signalHandler(int signal);
