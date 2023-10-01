#include "EventHandler.hpp"
#include "ServerManager.hpp"
#include "HttpRequestParser.hpp"

EventHandler::EventHandler(): _event_list(std::vector<t_kevent>(1)){}

EventHandler::~EventHandler(){}

void EventHandler::initialize()
{
	_kq = kqueue();
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
			if (hasDisconnected(serverList, i))
				continue ;
			else if (handleIncomingConnections(serverList, size, i)) {}
			else if (handleClientReadEvents(serverList, i)) {}
			else if (handleClientWriteEvents(serverList, i)) {}
		}
	}
	close(_kq);
}

bool	EventHandler::hasDisconnected(std::deque<Server> &serverList, ssize_t index)
{
	if (_event_list[index].flags & EV_EOF)
	{
		Client *client = (Client *)_event_list[index].udata;
		std::cerr << "Client has disconnected from the server\n";
		close(client->getClientSocket().getFd());
		serverList[client->getAssignedServer()].removeClient(*client);
		return (true);
	}
	return (false);
}

bool	EventHandler::handleClientReadEvents(std::deque<Server > &serverList, ssize_t index)
{
	if (_event_list[index].filter == EVFILT_READ)
	{
		Client *client = (Client *)_event_list[index].udata;
		client->recieve();
		if (client->hasClosed())
		{
			registerEvent(*client, EVFILT_READ, EV_DELETE);
			close(client->getClientSocket().getFd());
			serverList[client->getAssignedServer()].removeClient(*client);
		}
		else if (client->getParserResult() == ParserComplete)
		{
//			std::cout << client->getRequest().getBodySize() << " LENGTH: " << client->getRequest().getValueByKey("Content-Length") << std::endl;
			if (registerEvent(*client, EVFILT_READ, EV_DELETE) < 0)
				perror ("kevent [ EVREAD -> EV_DELETE ]");
			if (registerEvent(*client, EVFILT_WRITE, EV_ADD | EV_CLEAR) < 0)
				perror("kevent [ EV_WRITE ]");
		}
		return (true);
	}
	return (false);
}


std::string	getBoundary(const HttpRequest &request)
{
	std::string boundary;
	std::string boundaryText = "boundary=";
	std::string input = request.getValueByKey("Content-Type");
	size_t boundaryPos = input.find(boundaryText);
	if (boundaryPos != std::string::npos) {
		// Extract everything after the boundary
		boundary = input.substr(boundaryPos + boundaryText.length());
//		std::cout << "Extracted data: " << boundary << " position: " << boundaryPos << std::endl;
	} else {
		std::cerr << "Boundary not found in the input string." << std::endl;
	}
	return boundary;
}

void	uploadFile(const HttpRequest &request)
{
	std::string body;
		std::cerr << "started upload." << std::endl;
	std::string boundary = getBoundary(request);
	body = request.getFullBody();
//	std::cout << "body " << body << std::endl;
	std::string fileName = request.extractFileName(body);
	std::ofstream tempFile(fileName.c_str(), std::ios::binary | std::ios::trunc);
	if (!tempFile) {
		std::cerr << "Failed to create temporary file." << std::endl;
		tempFile.close();
		return ;
	}

	std::string pngDel = "\r\n\r\n";

	std::string::size_type startPos = body.find(pngDel);
	if (startPos == std::string::npos) {
		std::cerr << "Boundary not found in input data." << std::endl;
		tempFile.close();
		return ;
	}

	startPos += pngDel.length();

	std::string::size_type endPos = body.find("--" + boundary, startPos);
	if (endPos == std::string::npos) {
		std::cerr << "Ending boundary not found in input data." << std::endl;
		tempFile.close();
		return ;
	}

	std::string::size_type lastCRLF = body.rfind("\r\n", endPos);

	if (lastCRLF == std::string::npos) {
		std::cerr << "No '\\r\\n' found before the ending boundary." << std::endl;
		tempFile.close();
		return;
	}
	std::string bodyData = body.substr(startPos, lastCRLF - startPos);
	// Extract the binary data
		std::cerr << "started writing." << std::endl;
	tempFile.write(bodyData.c_str(), lastCRLF - startPos);
		std::cerr << "finished writing." << std::endl;
	tempFile.close();
}

bool	EventHandler::handleClientWriteEvents(std::deque<Server > &serverList, ssize_t index)
{
	if (_event_list[index].filter == EVFILT_WRITE)
	{
		Client *client = (Client *)_event_list[index].udata;
		if (client->getRequest().getRequestMethod() == "POST")
			uploadFile(client->getRequest());
//		HttpResponse response(client->getRequest(), serverList[client->getAssignedServer()].getServerConfig());
		HttpResponse test(client->getRequest(), serverList[client->getAssignedServer()].getConfiguration());
		client->send(test.getResponse().c_str(), test.getResponseSize());
		if (client->hasClosed())
		{
			registerEvent(*client, EVFILT_WRITE, EV_DELETE);
			close(client->getClientSocket().getFd());
			serverList[client->getAssignedServer()].removeClient(*client);
		}
		return (true);
	}
	return (false);
}

bool EventHandler::handleIncomingConnections(std::deque<Server> &serverList, ssize_t size, ssize_t i)
{
	if (size >= 0 && size < serverList.size() && _event_list[i].filter == EVFILT_READ && \
 		_event_list[i].ident == serverList[size].getSocket().getSocketFD())
	{
		serverList[size].acceptIncomingConnections(_kq);
		return (true);
	}
	return false;
}

int	EventHandler::registerEvent(Client &client, int16_t filter, uint16_t flags) const
{
	t_kevent	event;

	EV_SET(&event, client.getClientSocket().getFd(), filter, flags, 0, 0, &client);
	return (kevent(_kq, &event, 1, nullptr, 0, nullptr));
}
