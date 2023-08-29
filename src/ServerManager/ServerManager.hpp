#include <vector>
#include "Server.hpp"

class ServerManager{

  public:
	ServerManager();
	~ServerManager();

	void	initServers();
	void	runServers();

  private:
	std::vector<Server>	_serverList;
};