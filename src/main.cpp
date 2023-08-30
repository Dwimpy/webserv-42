#include "ServerManager.hpp"

int main (void)
{
	ServerManager	serverManager;
//	std::string request_msg = "GET /docs/index.html HTTP/1.1\r\nContent-Type: application/json\r\nContent-Type: application/json\r\nContent-Type: application/text\r\n";
//	HttpRequest request(request_msg);
	serverManager.buildServers("default.conf");
	if (serverManager.startServers())
		serverManager.runServers();
//	if (server.startServer())
//		server.run();
	return (0);
}
