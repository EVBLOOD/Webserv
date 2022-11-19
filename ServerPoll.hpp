#pragma once
#include "Request.hpp"
#include "Server.hpp"
#include "socket_header.hpp"

class ServerPoll {
    std::vector<Server> servers;
    struct pollfd fds[200];
    size_t num_of_servers;

   public:
    ServerPoll();
    void add_server(Server server);
    void run_servers();
};
