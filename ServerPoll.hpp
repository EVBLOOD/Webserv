#pragma once

#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "socket_header.hpp"

typedef struct pollfd pollfd;

class ServerPoll {
    std::vector <Server> _servers;
    std::vector <pollfd> _fds;
    std::vector <size_t> _owner;
    size_t _num_of_servers;
    size_t _num_of_clients;

public:
    ServerPoll();

    ServerPoll &add_server(const Server &server);

    void run_servers();

private:
    void add_client(int client_socket_fd, size_t server_index);

    void remove_client(size_t client_index);
};
