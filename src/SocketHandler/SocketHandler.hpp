#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <unordered_set>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "ServerConfig.hpp"
#include "unistd.h"

typedef struct pollfd		t_pollfd;
typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;

class SocketHandler{

  public:
	SocketHandler();
	~SocketHandler();

	bool			createSocket();
	bool			bindSocket(const t_sockaddr_in &serverAddr) const;
	bool			listenForClientConnections(const unsigned int &port);

	int				pollIncomingRequests();
	int				acceptIncomingRequest();
	void			removeClientAtIndexAndCloseFd(size_t &index);

	size_t			getActiveSocketsSize();

	bool			isFdServerSocket(const int &fd);
	const t_pollfd	&getClientAtIndex(size_t index) const;
	const int		&getServerSocket() const;
	const int		&getPollingStatus() const;
	void			cleanUpRemainingConnections();

	static bool		isReventError(const short &revents);
	static bool		isReventPolling(const short &revents);

  private:
	int						_serverSocket;
	int						_pollReady;
	std::unordered_set<int>	_connectedClients;
  	std::vector<t_pollfd>	_activeSockets;
};