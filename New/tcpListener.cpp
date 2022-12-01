#include "tcpListener.hpp"
#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>

// class TcpListener {
//   int fd;

// public:
//   TcpListener(std::string host, std::string port);
//   int get_raw_fd();
//   TcpStream accept();
// };

// EX :: TcpListener("localhost", 8080)
TcpListener::TcpListener(std::string host, std::string port)
    : _port(port), _host(host) {
    int backlog = SOMAXCONN;
    infos* addr;
    infos hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_protocol = IPPROTO_TCP;  // I think I can remove this one!
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &addr) == -1) {
        std::cerr << "get info is joking!\n";
        exit(1);
    }
    // struct addrinfo* res = addr;
    // char addrstr[100];
    // void* ptr;
    // {
    //     while (res) {
    //         inet_ntop(res->ai_family, res->ai_addr->sa_data, addrstr, 100);

    //         switch (res->ai_family) {
    //             case AF_INET:
    //                 ptr = &((struct sockaddr_in*)res->ai_addr)->sin_addr;
    //                 break;
    //             case AF_INET6:
    //                 ptr = &((struct sockaddr_in6*)res->ai_addr)->sin6_addr;
    //                 break;
    //         }
    //         inet_ntop(res->ai_family, ptr, addrstr, 100);
    //         printf("IPv%d address: %s (%s)\n",
    //                res->ai_family == PF_INET6 ? 6 : 4, addrstr,
    //                res->ai_canonname);
    //         res = res->ai_next;
    //     }
    // }
    _fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    int enable = 1;
    setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable));
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

IStreamer& TcpListener::accept() const {
    int client_sockfd = accept_helper(_fd);
    if (client_sockfd == -1) {
        std::cerr << "[ERROR] accept function failed : " << strerror(errno)
                  << '\n';
        exit(1);
    }
    TcpStream* new_client = new TcpStream(client_sockfd, *this);
    _clients.push_back(new_client);
    return *(new_client);
};

TcpListener::~TcpListener() {}

void TcpListener::delete_client(TcpStream* client) const {
    std::vector<TcpStream*>::iterator it =
        std::find(_clients.begin(), _clients.end(), client);
    assert(it != _clients.end());
    delete client;
    _clients.erase(it);
}

std::string TcpListener::get_port() const {
    return _port;
};
std::string TcpListener::get_host() const {
    return _host;
};
