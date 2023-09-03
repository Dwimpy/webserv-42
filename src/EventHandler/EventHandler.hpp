#include "Server.hpp"
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <vector>

typedef struct kevent t_kevent;

class EventHandler{

  public:
	EventHandler();
	~EventHandler();

	void	initialize();
	void	eventLoop(std::vector<Server > serverList);
	void	registerServers(std::vector<Server> &serverList);

  private:
	int			_kq;
	std::vector<t_kevent >	_monitor_list;
	std::vector<t_kevent > 	_event_list;

	bool	registerClientEvent(int clientfd);
	int		registerClientRemove(int fd);
	bool	doEventsServerSocket(std::vector<Server> &serverList, ssize_t &size, ssize_t &index);
	bool	doEventsClientSockets(std::vector<Server> &serverList, ssize_t index);
	bool	hasDisconnected(ssize_t index);
	void	removeDisconnectedClients(std::vector<Server> &serverList);
};