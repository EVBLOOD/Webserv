#include "Server.hpp"
#include "Request.hpp"
#include "Route.hpp"
#include "socket_header.hpp"

// void Post(Route& route, int status, float version, std::string body) {
//     std::string head =
//         "POST" + " " + route.get_location() assert(route.is_allowed_post());
// };

// class HttpRequest {};
// class HttpRespose {};

Server::Server()
        : _port(htons(0)),
          _host(htons(INADDR_ANY)),
          _socket_fd(-1),
          _host_addrlen(sizeof(struct sockaddr_in)),
          _backlog(SOMAXCONN) {}

Server &Server::init_socket() {
    std::cout << "[INFO] starting the server\n";
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket_fd == -1) {
        std::cerr << "[ERROR] socket function failed : " << strerror(errno) << std::endl;
        exit(1);
    }
    std::cout << "[INFO] a socket {ipv4, TCP} was created successfully\n";

    int enable = 1;

    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable,
                   sizeof(enable)) == -1) {
        std::cerr << "[ERROR] Setsockopt failed" << strerror(errno) << std::endl;
        exit(2);
    }

    // ***************

    _host_addr.sin_family = AF_INET;
    _host_addr.sin_port = _port;
    _host_addr.sin_addr.s_addr = _host;

    if (bind(_socket_fd, (struct sockaddr *) &_host_addr, _host_addrlen) == -1) {
        std::cerr << "bind function failed : " << strerror(errno) << std::endl;
        exit(3);
    }
    std::cout << "[INFO] socket was successfully bound port " << ntohs(_port) << " and host "
              << inet_ntoa(_host_addr.sin_addr)
              << '\n';
    // ***************
    if (listen(_socket_fd, _backlog) == -1) {
        std::cerr << "[ERROR] listen function failed : " << strerror(errno) << std::endl;
        exit(4);
    }
    std::cout << "[INFO] server is listening for connections\n";
    // ***************
    return *this;
}

int Server::get_socket_fd() const {
    return _socket_fd;
}

int Server::accept_connection() {
    int client_sockfd =
            accept(_socket_fd, (struct sockaddr *) &_host_addr, &_host_addrlen);
    if (client_sockfd == -1) {
        std::cerr << "[ERROR] accept function failed : " << strerror(errno) << '\n';
        return -1;
    }
    std::cout << "[INFO] connection is accepted\n";
    return client_sockfd;
}

Server::~Server() {
    close(_socket_fd);
}

in_port_t Server::getPort() const {
    return _port;
}

in_addr_t Server::getHost() const {
    return _host;
}

int Server::getBacklog() const {
    return _backlog;
}
