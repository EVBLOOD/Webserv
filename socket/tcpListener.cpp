#include "tcpListener.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>

// EX :: TcpListener("localhost", "8080")
TcpListener::TcpListener(std::string host, std::string port)
    : _port(port), _host(host) {
    int backlog = SOMAXCONN;
    infos* addr;
    infos hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;  // I think I can remove this one!
    hints.ai_flags = AI_PASSIVE;
    int ireturn = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &addr);
    if (ireturn == -1) {
        std::cerr << "get info is joking!\n";
        exit(1);
    }
    _fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    std::cout << "{SAAD IS A LIER} " << _fd << '\n';
    int enable = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) ==
        -1) {
        std::cerr << "Setsockopt failed" << strerror(errno) << std::endl;
        exit(-1);
    }
    if (bind(_fd, addr->ai_addr, addr->ai_addrlen) == -1) {
        std::cerr << "bind is joking!\n";
        exit(1);
    }
    if (listen(_fd, backlog) == -1) {
        std::cerr << "listen is joking!\n";
        exit(1);
    }
    freeaddrinfo(addr);
};

int TcpListener::get_raw_fd() const {
    return _fd;
};

int accept_helper(int fd) {
    return accept(fd, NULL, NULL);
}

TcpStream& TcpListener::accept() const {
    int client_sockfd = accept_helper(_fd);
    if (client_sockfd == -1) {
        std::cerr << "[ERROR] accept function failed : " << strerror(errno)
                  << '\n';
        exit(1);
    }
    TcpStream* new_client = new TcpStream(client_sockfd, *this);
    return *(new_client);
};

TcpListener::~TcpListener() {}

std::string TcpListener::get_port() const {
    return _port;
};
std::string TcpListener::get_host() const {
    return _host;
};
