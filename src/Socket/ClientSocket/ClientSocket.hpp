#include "ISocket.hpp"
#include <string>

class ClientSocket: public ISocket{

  public:
	ClientSocket();
	~ClientSocket();
	void	close();
	void	setFd(int fd);
	bool			receive();
	ssize_t			send(ssize_t bufferSize);

	t_sockaddr_in	&getClientAddr();
	socklen_t		&getClientAddrLen();
	const int		&getFd() const;
	unsigned char	*getBuffer();
	void			addToBuffer(const char *str, size_t size);

	ssize_t 		getBytesReceived();

  private:
	int				_fd;
	unsigned char	_buffer[1];
	char			_sendBuffer[1024];
	t_sockaddr_in	_clientAddr;
	socklen_t 		_clientAddrLen;
	ssize_t 		_bytes_received;
};
