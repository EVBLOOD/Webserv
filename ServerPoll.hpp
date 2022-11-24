#pragma once

#include "Request.hpp"
#include "HttpResponse.hpp"
#include "Server.hpp"
#include "socket_header.hpp"

typedef struct pollfd pollfd;

class ServerPoll {
    std::vector <Server> _servers;
    pollfd _fds[200];
    size_t _owner[200];
    size_t _num_of_servers;
    size_t _num_of_clients;

public:
    ServerPoll();

    void add_server(const Server &server);

    void run_servers();

    void add_client(int client_socket_fd, size_t server_index);

    void remove_client(size_t client_index);
};
