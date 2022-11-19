#include "ServerPoll.hpp"

ServerPoll::ServerPoll() : servers(), num_of_servers(0) {
    memset((void*)fds, 0, 200);
}

void ServerPoll::add_server(Server server) {
    servers.push_back(server);
    num_of_servers = servers.size();
}

void ServerPoll::run_servers() {
    int rc;
    char buffer[BUFFER_SIZE];

    for (size_t i = 0; i < num_of_servers; ++i) {
        servers[i].init_socket();
        fds[i].fd = servers[i].get_socket_fd();
        std::cout << "fd " << servers[i].get_socket_fd() << '\n';
        fds[i].events = POLLIN;
    }
    std::cout << "pool init\n";
    do {
        std::cout << (nfds_t)servers.size() << "  " << (nfds_t)num_of_servers
                  << " start" << std::endl;
        rc = poll(fds, (nfds_t)num_of_servers, -1);
        std::cout << "end" << std::endl;
        if (rc < 0) {
            perror("poll failed");
            break;
        }
        std::cout << "pool success" << std::endl;
        for (size_t i = 0; i < num_of_servers; ++i) {
            std::cout << "here";
            if (fds[i].revents == POLLIN) {
                memset((void*)buffer, 0, BUFFER_SIZE);
                int client_sockfd = servers[i].accept_connection();
                if (client_sockfd == -1) {
                    std::cerr << "accept function failed : " << strerror(errno)
                              << '\n';
                    continue;
                }
                std::cout << "connection is accepted\n";

                ssize_t bytes_read = read(client_sockfd, buffer, BUFFER_SIZE);
                if (bytes_read < 0) {
                    std::cerr << "read error : " << strerror(errno) << '\n';
                    continue;
                }

                std::cout << buffer << '\n';
                // ***************
                // std::string resp = Response(200, 1.1, "OK",
                //                             Body("text/html",
                //                                  "<h1>hello,
                //                                  world</1>\r\n"
                //                                  "<ul><li>13</li>\r\n"
                //                                  "<li>37</li></ul>\r\n"));
                std::vector<std::string> body_content;
                body_content.push_back("<h1>hello, world</1>");
                body_content.push_back("<ul><li>13</li>");
                body_content.push_back("<li>37</li></ul>");
                std::string resp = Server::Response(
                    301, 1.1, "Moved Permanently\r\nLocation: /test",
                    Body("text/html", body_content));
                ssize_t bytes_write =
                    write(client_sockfd, resp.c_str(), resp.size());
                if (bytes_write < 0) {
                    std::cerr << "write error : " << strerror(errno) << '\n';
                    continue;
                }
                close(client_sockfd);
            }
        }
        // ***************
    } while (true);
}
