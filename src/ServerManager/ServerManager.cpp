#include "ServerManager.hpp"
#include "Server.hpp"
#include <unistd.h>
#include <sys/event.h>

static bool	shouldExit = false;

ServerManager::ServerManager(): _serverConfigurator(ServerConfigurator())
{
	signal(SIGINT, signalHandler);
}

ServerManager::~ServerManager()
{}

void	ServerManager::buildServers(const std::string &filePath)
{
	parseConfigurationFile(filePath);
	createAndConfigureServers();
}

void ServerManager::pollIncomingConnections()
{

	try {
		this->_pollReady = poll(_activeSockets.data(), _activeSockets.size(), -1);
		if ((this->_pollReady < 0 && errno != EINTR) || this->_pollReady == 0)
			throw std::runtime_error("Polling encountered an error. Exiting");
	} catch (std::exception &e){
		std::cerr << "Caught exception: " << e.what() << std::flush;
		exit(1);
	}
}

void ServerManager::runServers()
{

	  int socket_listen_fd,
        portno = 1815,
        client_len,
        socket_connection_fd,
        kq,
        new_events;
    struct kevent change_event[25],
        event[4];
    struct sockaddr_in serv_addr,
        client_addr;
	int clientfd;
	kq = kqueue();

    // Create event 'filter', these are the events we want to monitor.
    // Here we want to monitor: socket_listen_fd, for the events: EVFILT_READ
    // (when there is data to be read on the socket), and perform the following
    // actions on this kevent: EV_ADD and EV_ENABLE (add the event to the kqueue
    // and enable it).
    EV_SET(change_event, _serverList[0].getSocket().getSocketFD(), EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);

    // Register kevent with the kqueue.
    if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1)
    {
        perror("kevent");
        exit(1);
    }

    // Actual event loop.
    for (;;)
    {
        // Check for new events, but do not register new events with
        // the kqueue. Hence the 2nd and 3rd arguments are NULL, 0.
        // Only handle 1 new event per iteration in the loop; 5th
        // argument is 1.
        new_events = kevent(kq, nullptr, 0, event, 1, nullptr);
        if (new_events == -1)
        {
            perror("kevent");
            exit(1);
        }

     for (int i = 0; new_events > i; i++)
    {
        int event_fd = event[i].ident;

        if (event[i].flags & EV_EOF)
        {
            printf("Client has disconnected\n");
            close(event_fd);
            continue;
        }
        else if (event_fd == _serverList[0].getSocket().getSocketFD())
        {
            _serverList[0].acceptIncomingConnections(kq, change_event);
            for (int j = 0; j < _serverList[0]._connectedClients.size(); ++j)
            {
                clientfd = _serverList[0]._connectedClients[j].getClientSocket().getFd();

                if (clientfd >= 0)
                {
                    struct kevent ev_add_client;
                    EV_SET(&ev_add_client, clientfd, EVFILT_READ, EV_ADD, 0, 0, NULL);
                    if (kevent(kq, &ev_add_client, 1, nullptr, 0, nullptr) < 0)
                    {
                        perror("kevent (EV_ADD client)");
                    }
                }
            }
        }
        else if (event[i].filter & EVFILT_READ)
        {
            _serverList[0].sendResponse(event_fd);
            close(event_fd);
            // Optionally, remove the event from the kqueue for this specific file descriptor
            struct kevent ev_remove_client;
            EV_SET(&ev_remove_client, event_fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
            kevent(kq, &ev_remove_client, 1, nullptr, 0, nullptr);
        }
    }
	_serverList[0]._connectedClients.clear();
}
}

void	ServerManager::handleServerRequests(t_pollfd *pollfd)
{
	if (pollfd->revents == POLLIN)
	{
		this->_serverList[0].sendResponse(*pollfd);
		pollfd->revents = 0;
		pollfd->fd = -1;
	}
}

void	ServerManager::removeFd(std::vector<t_pollfd> &fds)
{
	std::vector<t_pollfd >::const_iterator it = fds.cbegin();

	while (it != fds.cend())
	{
		if (it->fd == -1)
			it = fds.erase(it);
		else
			++it;
	}
}


bool ServerManager::startServers()
{
	for (size_t i = 0; i < _serverList.size(); ++i)
	{
		if (!_serverList[i].startServer())
			return (false);
		std::cout << "Server Started:\n   Listening on port: " << _serverList[i].getConfiguration().getPort() << std::endl;
		_activeSockets.push_back((t_pollfd){_serverList[i].getSocket().getSocketFD(), POLLIN, 0});
	}
	return (true);
}

void	ServerManager::parseConfigurationFile(const std::string &filePath)
{
	_serverConfigurator.parseConfigFile(filePath);
}

void	ServerManager::createAndConfigureServers()
{
	std::vector<ConfigFile>	configs;
	configs = _serverConfigurator.buildConfigFiles();

	for (size_t i = 0; i < configs.size(); ++i)
	{
		std::cout << "Server " << i + 1 << "\n";
		_serverList.__emplace_back(configs[i]);
		std::cout << "Port: " << _serverList.back().getConfiguration().getPort() << "\n";
		std::cout << "\n";
	}
}

void	signalHandler(int signal)
{
	if (signal == SIGINT)
		shouldExit = true;
}
