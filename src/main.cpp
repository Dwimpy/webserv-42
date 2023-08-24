#include "Server.hpp"


int main (void)
{
	Server server;

	if (server.startServer())
		server.run();
	return (0);
}
