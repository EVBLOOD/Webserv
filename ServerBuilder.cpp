#include "ServerBuilder.hpp"

ServerBuilder::ServerBuilder() : Server() {};

ServerBuilder &ServerBuilder::set_port(in_port_t port) {
    _port = htons(port);
    return *this;
}

ServerBuilder &ServerBuilder::set_host(in_addr_t host) {
    _host = htonl(host);
    return *this;
}

ServerBuilder &ServerBuilder::set_host(const std::string &host) {
    std::vector<std::string> splited = split(host, ".");
    unsigned char bytes[4];
    bytes[0] = std::stoi(splited[0]);
    bytes[1] = std::stoi(splited[1]);
    bytes[2] = std::stoi(splited[2]);
    bytes[3] = std::stoi(splited[3]);
    in_addr_t after = *(in_addr_t *) bytes;
    _host = after;
    return *this;
}

ServerBuilder &ServerBuilder::set_backlog(int backlog) {
    _backlog = backlog;
    return *this;
}

Server &ServerBuilder::build() {
    memset((void *) &_host_addr, 0, _host_addrlen);
    return *this;
}
