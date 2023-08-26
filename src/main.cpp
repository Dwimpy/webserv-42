#include "Server.hpp"

#include "HttpRequestParser.hpp"
#include <fstream>
#include <unistd.h>

int main (int argc, char **argv, char **env)
{
	Server server;
//	std::string request_msg = "GET /docs/index.html HTTP/1.1\r\nContent-Type: application/json\r\nContent-Type: application/json\r\nContent-Type: application/text\r\n";
//	HttpRequest request(request_msg);

//	std::string test;
	if (server.startServer())
		server.run();
	return (0);
}
