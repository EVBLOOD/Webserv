#pragma once

#include <string>
#include <vector>
#include "Request.hpp"
#include "Route.hpp"
#include "socket_header.hpp"

class Server {
private:
    std::vector<Route> routes;

protected:
public:
    in_port_t getPort() const;

    in_addr_t getHost() const;

protected:
    in_port_t _port;
    in_addr_t _host;
    int _socket_fd;
    struct sockaddr_in _host_addr;
    socklen_t _host_addrlen;
    int _backlog;
public:
    int getBacklog() const;

protected:

    Server();

public:
    Server &init_socket();

    int get_socket_fd() const;

    int accept_connection();

    ~Server();
};
