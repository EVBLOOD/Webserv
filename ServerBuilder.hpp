#pragma once
#include "Server.hpp"

class ServerBuilder : protected Server {
   public:
    ServerBuilder();
    ServerBuilder& set_port(in_port_t port);
    ServerBuilder& set_host(in_addr_t host);
    ServerBuilder& set_host(std::string host);
    ServerBuilder& set_backlog(int backlog);
    Server& build();
};
