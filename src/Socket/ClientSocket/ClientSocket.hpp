#include "ISocket.hpp"
#include <string>

class ClientSocket: public ISocket{

  public:
	ClientSocket();
	~ClientSocket();
	void	close();
	void	setFd(int fd);
	ssize_t			receive();
	ssize_t			send(const char *str, ssize_t bufferSize);

	t_sockaddr_in	&getClientAddr();
	socklen_t		&getClientAddrLen();
	const int		&getFd() const;
	char			*getBuffer();
	void			addToBuffer(const char *str, size_t size);
	void			setOffset(ssize_t n);
	ssize_t			getOffset();

  private:
	int				_fd;
	char			_buffer[1024000];
	ssize_t			_data_offset;
	t_sockaddr_in	_clientAddr;
	socklen_t 		_clientAddrLen;
};
