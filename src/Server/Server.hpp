#pragma once

# include "ServerConfig.hpp"
# include "SocketHandler.hpp"
# include "HttpRequestHandler.hpp"
# include <netinet/in.h>
# include <vector>

typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;


static bool	shouldExit = false;

class Server {

	typedef struct socketContext
	{
		t_sockaddr_in			serverAddr;
		t_sockaddr_in			clientAddrs;
		socklen_t				clientLen;
		size_t					receivedBytes;
	}				t_socketContext;

  public:
	Server();
	Server(const ServerConfig &config);
	~Server();
	bool	startServer();
	void	run(void);
	void	handleIncomingRequests();

  private:
	char					_buffer[8192];
	t_socketContext 		_socketContext;
	SocketHandler			_socketHandler;
	HttpRequestHandler		_httpRequestHandler;
	const ServerConfig		_config;

	void					initServerAddr();
};

void	signalHandler(int signal);