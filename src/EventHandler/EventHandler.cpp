#include "EventHandler.hpp"
#include "ServerManager.hpp"

EventHandler::EventHandler(): _event_list(std::vector<t_kevent>(1)){}

EventHandler::~EventHandler(){}

void EventHandler::initialize()
{
	_kq = kqueue();
}

void	EventHandler::registerServers(std::vector<Server> &serverList)
{
 	_monitor_list = std::vector<t_kevent >(serverList.size());
	for (ssize_t i = 0; i < serverList.size(); ++i)
		EV_SET(&_monitor_list[i], serverList[i].getSocket().getSocketFD(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (::kevent(_kq, this->_monitor_list.data(), serverList.size(), nullptr, 0, nullptr) < 0)
	{
		perror("kevent");
		exit(EXIT_FAILURE);
	}
}


void	EventHandler::registerServers(std::deque<Server> &serverList)
{
 	_monitor_list = std::vector<t_kevent >(serverList.size());
	for (ssize_t i = 0; i < serverList.size(); ++i)
		EV_SET(&_monitor_list[i], serverList[i].getSocket().getSocketFD(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
	if (::kevent(_kq, this->_monitor_list.data(), serverList.size(), nullptr, 0, nullptr) < 0)
	{
		perror("kevent");
		exit(EXIT_FAILURE);
	}
}


bool	EventHandler::registerClientEvent(Client &client) const
{
	t_kevent	add_client_event;

	EV_SET(&add_client_event, client.getClientSocket().getFd(), EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, &client);
	if (kevent(_kq, &add_client_event, 1, nullptr, 0, nullptr) < 0)
		return (false);
	return (true);
}

void	EventHandler::eventLoop(std::deque<Server > serverList)
{
	ssize_t size;
	int		new_events;
	struct kevent event_fd;

	size = 0;
	for (;;)
	{
		new_events = kevent(_kq, nullptr, 0, _event_list.data(), 1, nullptr);

		if (new_events == -1 && errno == EINTR)
		{
			perror("kevent");
			break ;
		}

		for (ssize_t i = 0; i < new_events; ++i)
		{
			event_fd = _event_list[i];
			size = event_fd.ident - 3;

			if (hasDisconnected(i))
				continue ;
			else if (doEventsServerSocket(serverList, size, i)){}
			else if (doEventsClientSockets(serverList, i)){}

		}
		removeDisconnectedClients(serverList);
	}
	close(_kq);
}

bool	EventHandler::hasDisconnected(ssize_t index)
{
	if (_event_list[index].flags & EV_EOF)
	{
		std::cerr << "Client has disconnected from the server\n";
		close(_event_list[index].ident);
		_event_list[index].ident = -1;
		return true;
	}
	return (false);

}

bool	EventHandler::doEventsServerSocket(std::deque<Server> &serverList, ssize_t &size, ssize_t &index)
{
	if (_event_list[index].flags & EVFILT_READ && (size >= 0 && size < serverList.size()) && \
		_event_list[index].ident == serverList[size].getSocket().getSocketFD())
	{
		serverList[size].acceptIncomingConnections(_kq, _monitor_list.data());
		Client *client = &serverList[size].getConnectedClients().back();
		if (!registerClientEvent(*client))
			perror("kevent [ add_client ]");
		return (true);
	}
	return (false);
}

bool	EventHandler::doEventsClientSockets(std::deque<Server> &serverList, ssize_t index)
{
	if (_event_list[index].ident & EVFILT_READ)
	{
		Client *the_client = (Client *)_event_list[index].udata;
		serverList[the_client->getAssignedServer()].sendResponse(*the_client);
		if (registerClientRemove(*the_client) > 0)
			perror("kevent [ EV_DELETE ]");
		the_client->setClientFd(-1);
		return (true);
	}
	return (false);
}

int	EventHandler::registerClientRemove(Client &client) const
{
	struct kevent ev_remove_client = {};
	EV_SET(&ev_remove_client, client.getClientSocket().getFd(), EVFILT_READ, EV_DELETE, 0, 0, &client);
	return (kevent(_kq, &ev_remove_client, 1, nullptr, 0, nullptr));

}

void	EventHandler::removeDisconnectedClients(std::deque<Server> &serverList)
{
	for (ssize_t i = 0; i < serverList.size(); ++i)
		serverList[i].removeClient();
}
