
#include "ServerManager.hpp"
#include "Server.hpp"
#include "HttpRequestParser.hpp"
#include <fstream>
#include <unistd.h>

int main (int argc, char **argv, char **env)
{
	ServerManager	serverManager;
	serverManager.buildServers("default.conf");
	if (serverManager.startServers())
		serverManager.runServers();
	return (0);
}
