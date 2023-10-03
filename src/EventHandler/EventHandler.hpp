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
	void	eventLoop(std::deque<Server > serverList);
	void	registerServers(std::deque<Server> &serverList);
	int		registerEvent(Client &client, int16_t filter, uint16_t flags) const;

  private:
	int			_kq;
	std::vector<t_kevent >	_monitor_list;
	std::vector<t_kevent > 	_event_list;

	bool	hasDisconnected(std::deque<Server> &serverList, ssize_t index);
	bool	handleIncomingConnections(std::deque<Server> &deque1, ssize_t size, ssize_t i);
	bool	handleClientReadEvents(std::deque<Server> &serverList, ssize_t index);
	bool	handleClientWriteEvents(std::deque<Server> &serverList, ssize_t index);
};

void	uploadFile(const HttpRequest &request);