#include "tcpListener.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <cstring>

static void shutdown_helper(int fd) {
    // return;
    shutdown(fd, SHUT_RDWR);
    close(fd);
}

TcpListener::~TcpListener() {
    shutdown_helper(_fd);
}

// EX :: TcpListener("localhost", "8080")
TcpListener::TcpListener(std::string host, std::string port)
    : _port(port), _host(host) {
    int backlog = SOMAXCONN;
    infos* addr;
    infos hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    int ireturn = ::getaddrinfo(host.c_str(), port.c_str(), &hints, &addr);
    if (ireturn != 0) {
        std::cerr << G(ERROR) << " getaddrinfo : " << strerror(errno) << '\n';
        exit(1);
    }
    _fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    int enable = 1;
    if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) ==
        -1) {
        std::cerr << G(ERROR) << " setsockopt : " << strerror(errno) << '\n';
        exit(1);
    }
    if (bind(_fd, addr->ai_addr, addr->ai_addrlen) == -1) {
        std::cerr << G(ERROR) << " bind : " << strerror(errno) << '\n';
        exit(1);
    }
    if (listen(_fd, backlog) == -1) {
        std::cerr << G(ERROR) << " listen : " << strerror(errno) << '\n';
        exit(1);
    }
    freeaddrinfo(addr);
};

int TcpListener::get_raw_fd() const {
    return _fd;
};

Kevent TcpListener::get_kevent() const {
    return _event;
};

void TcpListener::set_kevent(Kevent kv) {
    memset(&_event, 0, sizeof(Kevent));
    _event = kv;
}

int accept_helper(int fd) {
    return accept(fd, NULL, NULL);
}

TcpStream& TcpListener::accept() const {
    int client_sockfd = accept_helper(_fd);
    if (client_sockfd == -1) {
        std::cerr << G(ERROR) << " accept : " << strerror(errno)
                  << '\n';
        exit(1);
    }
    TcpStream* new_client = new TcpStream(client_sockfd, *this);
    return *(new_client);
};

std::string TcpListener::get_port() const {
    return _port;
};
std::string TcpListener::get_host() const {
    return _host;
};
