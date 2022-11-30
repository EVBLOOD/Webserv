#pragma once
#include <fcntl.h>
#include <malloc/_malloc.h>
#include <netdb.h>
#include <netinet/in.h>
#include <strings.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_socklen_t.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/syslimits.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <system_error>
#include <vector>
typedef struct addrinfo infos;
typedef struct sockaddr_storage storage;
#include <sys/event.h>
#include <sys/time.h>

#ifndef SOMAXCONN
#define SOMAXCONN 128
#endif

#include "TcpStream.hpp"
#include "server_interface.hpp"
class TcpListener : public IServer {
    int _fd;

   public:
    TcpListener(std::string host, std::string port);
    ~TcpListener();
    int get_raw_fd() const;
    TcpStream& accept() const;
};
