#include "ISocket.hpp"
#include <string>

class ClientSocket: public ISocket{

  public:
	ClientSocket();
	~ClientSocket();
	void	close();
	void	setFd(int fd);
	bool			receive();
	ssize_t			send();

	t_sockaddr_in	&getClientAddr();
	socklen_t		&getClientAddrLen();
	const int		&getFd() const;
	char			*getBuffer();
	void			addToBuffer(const char *str, size_t size);
  private:
	int				_fd;
	char			_buffer[8];
	t_sockaddr_in	_clientAddr;
	socklen_t 		_clientAddrLen;
};
