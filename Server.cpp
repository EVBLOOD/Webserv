#include "Server.hpp"
#include "Request.hpp"
#include "Route.hpp"
#include "socket_header.hpp"

std::string Server::Response(int status,
                             std::string version,
                             std::string action,
                             Body body) {
    std::string header1 = "HTTP/" + version + " " +
                          std::to_string(status) + " " + action + "\r\n";
    std::string header2 = "Server: webserver-c\r\n";
    std::string header3 = "Connection : close\r\n";
    std::string header4 = "Content-type: " + body.content_type + "\r\n\r\n";
    std::string body_content;
    for (size_t i = 0; i < body.content.size(); ++i) {
        body_content += (body.content[i] + "\r\n");
    }
    // "HTTP/1.0 200 OK\r\n"
    //   "Server: webserver-c\r\n"
    //   "Content-type: text/html\r\n\r\n"
    //   "<h1>hello, world</1>\r\n"
    //   "<ul><li>13</li>\r\n"
    //   "<li>37</li></ul>\r\n";
    std::cout << "[DEBUG]\n" << header1 + header2 + header3 + header4 + body_content;
    return header1 + header2 + header3 +  header4 + body_content;
};
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

Server& Server::init_socket() {
    std::cout << "starting the server\n";
    _socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_socket_fd == -1) {
        std::cerr << "socket function failed : " << strerror(errno) << '\n';
        exit(1);
    }
    std::cout << "socket created successfully\n";

    int enable = 1;

    if (setsockopt(_socket_fd, SOL_SOCKET, SO_REUSEADDR, &enable,
                   sizeof(enable)) == -1) {
        std::cerr << "Setsockopt failed" << strerror(errno) << std::endl;
        exit(-1);
    }

    // ***************

    _host_addr.sin_family = AF_INET;
    _host_addr.sin_port = _port;
    _host_addr.sin_addr.s_addr = _host;

    if (bind(_socket_fd, (struct sockaddr*)&_host_addr, _host_addrlen) == -1) {
        std::cerr << "bind function failed : " << strerror(errno) << '\n';
        exit(1);
    }
    std::cout << "socket was successfully binded to an address\n";
    // ***************
    if (listen(_socket_fd, _backlog) == -1) {
        std::cerr << "listen function failed : " << strerror(errno) << '\n';
        exit(1);
    }
    std::cout << "server is listening for connections\n";
    // ***************
    return *this;
}

int Server::get_socket_fd() {
    return _socket_fd;
}

int Server::accept_connection() {
    int client_sockfd =
        accept(_socket_fd, (struct sockaddr*)&_host_addr, &_host_addrlen);
    if (client_sockfd == -1) {
        std::cerr << "accept function failed : " << strerror(errno) << '\n';
        return -1;
    }
    std::cout << "connection is accepted\n";
    return client_sockfd;
}

Server::~Server() {
    close(_socket_fd);
}
