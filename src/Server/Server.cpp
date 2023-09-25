#include "Server.hpp"

#include <sys/event.h>
#include <unistd.h>
#include <algorithm>

Server::Server() : _config(ServerConfig())
{}

Server::Server(const ServerConfig &config) : _config(config)
{}

Server::Server(const ConfigFile &config): _configFile(config), _config(ServerConfig())
{}

Server::~Server()
{
//	close(_serverFd);
}

bool Server::startServer()
{
	if (!_serverSocket.create(_configFile.getPort())||
		!_serverSocket.bind() ||
		!_serverSocket.listen())
		return (false);
	return (true);
}


void Server::acceptIncomingConnections(int kq, struct kevent change[25])
{
	int		clientFd;
	size_t	i;

	i = 0;
	clientFd = 0;
	Client	newClient;
	clientFd = _serverSocket.accept(newClient);
	if (clientFd <= 0)
		return ;
	newClient.setServer(this->_serverSocket.getSocketFD() - 3);
	this->_connectedClients.push_back(newClient);
	i++;
}

void	Server::removeClient()
{
	for (std::deque<Client>::iterator it = _connectedClients.begin(); it != _connectedClients.end();)
	{
		if (it->getClientSocket().getFd() == -1)
			it = _connectedClients.erase(it);
		else
			++it;
	}
}

void	Server::sendResponse(int fd)
{
	ssize_t	bytes_received;
	memset(_buffer, 0, sizeof(_buffer));

	if ((bytes_received = recv(fd, _buffer, sizeof(_buffer) - 1, 0)) < 0)
	{
		perror("Error receiving data");
		std::cout << "SERVER RECV: fd: " << fd << std::endl;
	}
	else if (bytes_received == 0)
	{
		perror("ERROR connection closed by client");
	}
	else
	{
//		std::cout << _buffer << std::endl;
		std::string response_msg = std::string(_buffer);
		HttpRequest request(response_msg);
		HttpResponse responseObj(request, _config);
		std::string response = responseObj.getResponse();
		bytes_received = send(fd, response.c_str(), response.size(), 0);
		if (bytes_received < 0)
			perror("ERROR socket closed");
	}
}

std::string	extractFileName(std::string &body)
{
	std::string filename = "untitled.txt";

	size_t filenamePos = body.find("filename=");

	if (filenamePos != std::string::npos) {
		// Move the position to the start of the filename
		filenamePos += 10; // "filename=" is 10 characters long

		// Find the closing double-quote (") after the filename
		size_t closingQuotePos = body.find("\"", filenamePos);

		if (closingQuotePos != std::string::npos) {
			// Extract the filename between the double-quotes
			filename = body.substr(filenamePos, closingQuotePos - filenamePos);
			std::cout << "Extracted filename: " << filename << std::endl;
		} else {
			std::cerr << "Closing double-quote not found." << std::endl;
		}
	} else {
		std::cerr << "Filename not found in the data." << std::endl;
	}
	return filename;
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
		std::cout << "Extracted data: " << boundary << " position: " << boundaryPos << std::endl;
	} else {
		std::cerr << "Boundary not found in the input string." << std::endl;
	}
	return boundary;
}

void	uploadFile(const HttpRequest &request)
{
	std::string body;
	std::string boundary = getBoundary(request);
	body = request.getFullBody();
	std::string fileName = extractFileName(body);

	std::ofstream tempFile(fileName.c_str(), std::ios::binary | std::ios::trunc);

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

	// Extract the binary data
	std::string binaryData = body.substr(startPos, lastCRLF - startPos);

	if (!tempFile) {
		std::cerr << "Failed to create temporary file." << std::endl;
		tempFile.close();
		return ;
	}
	tempFile.write(binaryData.c_str(), lastCRLF - startPos);
	tempFile.close();
}



void	Server::sendResponse(Client client)
{
	memset(_buffer, 0, sizeof(_buffer));
	std::string	request_msg;

	client.recieve();
	HttpResponse responseObj(client.getRequest(), _config);
	request_msg = client.getRequest().getFullBody();

	if (client.getRequest().getRequestMethod() == "POST")
	{
		uploadFile(client.getRequest());
		//		int fd = open("compare.png", O_RDWR | O_CREAT | O_TRUNC, 0644);
//		std::vector<char> test(request_msg.begin() + 137, request_msg.end() - 44);
//		for (char c : test) {
//			std::cout << c << std::endl;
//			write(fd, &c, 1);
//		}
//		close(fd);
	}
	client.send(responseObj.getResponse().c_str(), responseObj.getResponseSize());
	close(client.getClientSocket().getFd());

}

const ConfigFile &Server::getConfiguration() const
{
	return (this->_configFile);
}

ServerSocket Server::getSocket() const
{
	return (this->_serverSocket);
}
std::deque<Client> &Server::getConnectedClients()
{
	return (this->_connectedClients);
}
const ConfigFile &Server::getConfiguration()
{
	return (this->_configFile);
}
