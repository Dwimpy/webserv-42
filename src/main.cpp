#include "Server.hpp"

#include "HttpRequestParser.hpp"

int main (void)
{
	Server server;
	std::string request_msg = "GET /favicon HTTP/1.1/r/n";
	HttpRequest request(request_msg);

//	std::string test;
	if (server.startServer())
		server.run();
	return (0);
}
