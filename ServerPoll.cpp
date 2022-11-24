#include "ServerPoll.hpp"

ServerPoll::ServerPoll() : _servers(), _num_of_servers(0), _num_of_clients(0) {
    memset((void *) _fds, 0, 200);
}

void ServerPoll::add_server(const Server &server) {
    _servers.push_back(server);
    _num_of_servers = _servers.size();
}

HttpResponse test_response(HttpRequest request) {
    std::cerr << "[DEBUG] test_response\n";
    std::string location = request.getLocation();

    if (location == "/") {
        return HttpResponse(200, "1.1", "OK\r\nLocation: /")
                .add_header("Content-Type", "text/html")
                .add_to_body("<h1>hello, world</1>");
    }
    if (location == "/index.html") {
        return HttpResponse(200, "1.1", "OK\r\nLocation: /index.html")
                .add_header("Content-Type", "text/html")
                .set_body(tools::open_to_serve("index.html"));
    }
    if (location == "/app.css") {
        return HttpResponse(200, "1.1", "OK\r\nLocation: /index.html")
                .add_header("Content-Type", "text/css")
                .set_body(tools::open_to_serve("app.css"));
    }
    if (location == "/app.js") {
        return HttpResponse(200, "1.1", "OK\r\nLocation: /index.html")
                .add_header("Content-Type", "application/javascript")
                .set_body(tools::open_to_serve("app.js"));
    }
    if (location == "/oussama" && request.getMethod() == "GET") {
        return HttpResponse(200, "1.1", "OK\r\nLocation: /oussama")
                .add_header("Content-Type", "text/html")
                .add_to_body(" <form  method = \"POST\">")
                .add_to_body("Name: <input type = \"text\" name = \"name\" />")
                .add_to_body("Weight: <input type = \"text\" name = \"weight\" />")
                .add_to_body("<input type = \"submit\" />")
                .add_to_body("</form>");
    }
    if (location == "/oussama" && request.getMethod() == "POST") {
        std::string content = request.getBody()[0];
        std::vector <std::string> name_weight = split(content, "&");
        return HttpResponse(200, "1.1", "OK\r\nLocation: /oussama")
                .add_header("Content-Type", "text/html")
                .add_to_body("<h1>hello " + split(name_weight[0], "=")[1] + "</h1>")
                .add_to_body("<h1>your weight is " + split(name_weight[1], "=")[1] + "</h1>");
    }
    if (location == "/saad") {
        return HttpResponse(
                301, "1.1", "Moved Permanently\r\nLocation: /oussama").
                add_header("Location", "/oussama");
    }

    return HttpResponse(404, "1.1", "Not Found\r\nLocation: /")
            .add_header("Content-Type", "text/html")
            .add_to_body("<h1>404</1>");
}


void ServerPoll::run_servers() {
    int rc;
    char buffer[BUFFER_SIZE];

    for (size_t i = 0; i < _num_of_servers; ++i) {
        _servers[i].init_socket();
        _fds[i].fd = _servers[i].get_socket_fd();
        _fds[i].events = POLLIN;
    }

    do {
        rc = poll(_fds, (nfds_t)(_num_of_servers + _num_of_clients), -1);
        std::cout << "[DEBUG POLL] " << rc << "\n";
        if (rc < 0) {
            perror("poll failed");
            break;
        }
        std::cout << "pool success" << std::endl;
        for (size_t i = 0; i < _num_of_servers + _num_of_clients; ++i) {
            if (_fds[i].revents == POLLIN) {
                std::cout << "[DEBUG] POLLIN event in the server " << i << std::endl;
                if (i < _num_of_servers) {
                    int client_socket_fd = _servers[i].accept_connection();
                    if (client_socket_fd == -1) {
                        std::cerr << "accept function failed : " << strerror(errno)
                                  << '\n';
                        continue;
                    }
                    std::cout << "connection is accepted\n";
                    add_client(client_socket_fd, i);
                    continue;
                }


                std::cout << "[DEBUG] POLLIN event in the client " << i << std::endl;
                memset((void *) buffer, 0, BUFFER_SIZE);
                ssize_t bytes_read = read(_fds[i].fd, buffer, BUFFER_SIZE);
                if (bytes_read <= 0) {
                    remove_client(i);
                    if (bytes_read < 0)
                        std::cerr << "read error : " << strerror(errno) << '\n';
                    continue;
                }

                std::cout << "[REQUEST from client " << i << "]\n";
                HttpRequest request = HttpRequest(std::string(buffer));
//                std::cout << "[DEBUG] raw buffer data start :\n[" << buffer << "]\n"
//                          << "[DEBUG] raw buffer data ends" << std::endl;

                HttpResponse resp = test_response(request);

                ssize_t bytes_write =
                        write(_fds[i].fd, resp.build().c_str(), resp.build().size());
                if (bytes_write < 0) {
                    std::cerr << "write error : " << strerror(errno) << '\n';
                    continue;
                }

                std::cout << "[DEBUG REQUEST DUMB]\n";
                request.dump();
                std::cout << "[END REQUEST DUMB]\n";
                if (true || request.getHeaderValue("Connection") == "close") {
                    remove_client(i);
                } else if (request.getHeaderValue("Connection") == "keep-alive") {
                    std::cout << "client " << i << " want to be kept alive\n";
                }

            }

            std::cerr << "[ERROR] " << strerror(errno) << std::endl;
            errno = 0;
            std::cerr << "[DEBUG] index : " << i << " and revent " << _fds[i].revents << std::endl;
//            switch (_fds[i].revents) {
//                case POLLERR:
//                    std::cerr << "POLLERR\n";
//                    break;
//                case POLLHUP:
//                    std::cerr << "POLLHUP\n";
//                    remove_client(i);
//                    std::cout << "close connection with client " << i << "\n";
//                    break;
//                case POLLIN:
//                    std::cerr << "POLLIN\n";
//                    break;
//                case POLLNVAL:
//                    std::cerr << "POLLNVAL\n";
//                    break;
//                case POLLOUT:
//                    std::cerr << "POLLOU\n";
//                    break;
//                case POLLPRI:
//                    std::cerr << "POLLPRI\n";
//                    break;
//                case POLLRDBAND:
//                    std::cerr << "POLLRDBAND\n";
//                    break;
//                case POLLRDNORM:
//                    std::cerr << "POLLRDNOR\n";
//                    break;
//                case POLLWRBAND:
//                    std::cerr << "POLLWRBAND\n";
//                    break;
////                case POLLWRNORM:
////                    std::cerr << "POLLWRNORM\n";
////                    break;
//                case 0:
//                    continue;
//                default:
//                    std::cerr << "UNREACHABLE\n";
//                    remove_client(i);
//            }
        }
        // ***************
    } while (true);
}

//
void ServerPoll::add_client(int client_socket_fd, size_t server_index) {
    _owner[_num_of_servers + _num_of_clients] = server_index;
    _fds[_num_of_servers + _num_of_clients].fd = client_socket_fd;
    _fds[_num_of_servers + _num_of_clients].events = POLLIN;
    ++_num_of_clients;
}

//
void ServerPoll::remove_client(size_t client_index) {
    close(_fds[client_index].fd);
    if (client_index != _num_of_servers) {
        _owner[client_index] = _owner[_num_of_servers + --_num_of_clients];
        _fds[client_index] = _fds[_num_of_servers + --_num_of_clients];
    } else
        _num_of_clients = 0;
    std::cerr << "[INFO] close connection with client " << client_index << std::endl;
}
