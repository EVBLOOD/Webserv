#include "tcpListener.hpp"
#include <sys/socket.h>

// class TcpListener {
//   int fd;

// public:
//   TcpListener(std::string host, std::string port);
//   int get_raw_fd();
//   TcpStream accept();
// };

// EX :: TcpListener("localhost", 8080)
TcpListener::TcpListener(std::string host, std::string port) {
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

TcpStream& TcpListener::accept() const {
    int client_sockfd = accept_helper(_fd);
    if (client_sockfd == -1) {
        std::cerr << "[ERROR] accept function failed : " << strerror(errno)
                  << '\n';
        exit(1);
    }
    std::cout << "[INFO] connection is accepted\n";
    return *(new TcpStream(client_sockfd));
};

TcpListener::~TcpListener() {}
