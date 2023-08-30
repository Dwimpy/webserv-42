#pragma once
#include <netinet/in.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <exception>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

typedef struct pollfd		t_pollfd;
typedef struct sockaddr_in	t_sockaddr_in;
typedef struct sockaddr		t_sockaddr;

class ISocket{

  public:
	virtual ~ISocket(){};
	virtual void close() = 0;
};