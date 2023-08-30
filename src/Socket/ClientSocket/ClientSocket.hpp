#include "ISocket.hpp"


class ClientSocket: public ISocket{

  public:
	ClientSocket();
	~ClientSocket();
	void	close();
	void	setFd(int fd);

	t_sockaddr_in	&getClientAddr();
	socklen_t		&getClientAddrLen();
	const int		&getFd() const;

  private:
	int				_fd;
	t_sockaddr_in	_clientAddr;
	socklen_t 		_clientAddrLen;
};