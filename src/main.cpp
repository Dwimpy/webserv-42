#include "Server.hpp"

#include "HttpRequestParser.hpp"
#include <fstream>


int main (void)
{
//    std::string response;
//    std::ifstream	infile("docs/index.html");
//
//    if (!infile.good()){
//        perror("error opening file");
//        exit (1);
//    }
//    response.append("HTTP/1.0 200 OK\r\n"
//                    "Content-Type: text/html\r\n"
//                    "\r\n");
//    char c;
//    while(infile.get(c))
//        response.push_back(c);
	Server server;
//	std::string request_msg = "GET /docs/index.html HTTP/1.1\r\nContent-Type: application/json\r\nContent-Type: application/json\r\nContent-Type: application/text\r\n";
//	HttpRequest request(request_msg);

	std::string test;
	if (server.startServer())
		server.run();
	return (0);
}
