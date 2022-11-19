#include "ServerBuilder.hpp"
ServerBuilder::ServerBuilder() : Server(){};
ServerBuilder& ServerBuilder::set_port(in_port_t port) {
    _port = htons(port);
    return *this;
}
ServerBuilder& ServerBuilder::set_host(in_addr_t host) {
    _host = htonl(host);
    return *this;
}
ServerBuilder& ServerBuilder::set_backlog(int backlog) {
    _backlog = backlog;
    return *this;
}
Server& ServerBuilder::build() {
    memset((void*)&_host_addr, 0, _host_addrlen);
    return *this;
}
