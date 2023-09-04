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

bool	EventHandler::registerClientEvent(int clientfd)
{
	t_kevent	add_client_event;

	EV_SET(&add_client_event, clientfd, EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, NULL);
	if (kevent(_kq, &add_client_event, 1, nullptr, 0, nullptr) < 0)
		return (false);
	return (true);
}

void	EventHandler::eventLoop(std::vector<Server > serverList)
{
	ssize_t size;
	int		new_events;
	int		event_fd;

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
			event_fd = _event_list[i].ident;
			size = event_fd - 3;

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
		return true;
	}
	return (false);

}

bool	EventHandler::doEventsServerSocket(std::vector<Server> &serverList, ssize_t &size, ssize_t &index)
{
	int	clientfd;


	if (_event_list[index].flags & EVFILT_READ && (size >= 0 && size < serverList.size()) && _event_list[index].ident == serverList[size].getSocket().getSocketFD())
	{
		serverList[size].acceptIncomingConnections(_kq, _monitor_list.data());
		for (ssize_t i = 0; i < serverList[size].getConnectedClients().size(); ++i)
		{
			clientfd = serverList[size].getConnectedClients()[i].getClientSocket().getFd();

			if (clientfd >= 0)
				if (!registerClientEvent(clientfd))
					perror("kevent [ add_client ]");
		}
		return (true);
	}
	return (false);
}

bool	EventHandler::doEventsClientSockets(std::vector<Server> &serverList, ssize_t index)
{
	ssize_t		server_number;
	Client		client;

	if (_event_list[index].ident & EVFILT_READ)
	{
		server_number = ServerManager::findServerFromFd(serverList, client, _event_list[index].ident);
		serverList[server_number].sendResponse(client);
		if (registerClientRemove(_event_list[index].ident) < 0)
			perror("kevent [ EV_DELETE ]");
		close(_event_list[index].ident);
		return (true);
	}
	return (false);
}

int	EventHandler::registerClientRemove(int fd)
{
	struct kevent ev_remove_client;
	EV_SET(&ev_remove_client, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
	return (kevent(_kq, &ev_remove_client, 1, nullptr, 0, nullptr));

}

void	EventHandler::removeDisconnectedClients(std::vector<Server> &serverList)
{
	for (ssize_t i = 0; i < serverList.size(); ++i)
		serverList[i].removeClient();
}
