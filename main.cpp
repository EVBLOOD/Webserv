#include <stdlib.h>
#include <sys/_types/_size_t.h>
#include <sys/_types/_ssize_t.h>
#include <sys/event.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#include <unistd.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#include "Request.hpp"
#include "Response.hpp"
#include "parsing/location.hpp"
#include "parsing/parser.hpp"
#include "parsing/serverInfo.hpp"
#include "parsing/tokengen.hpp"
#include "socket/TcpStream.hpp"
#include "socket/kqueue.hpp"
#include "socket/listener_interface.hpp"
#include "socket/tcpListener.hpp"
#include "tools.hpp"

#define loop for (;;)
#define IF_NOT(cond) if (!(cond))
using namespace tools;
using std::array;
using std::cerr;
using std::cout;
using std::endl;
using std::list;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

// void print(std::string& type, std::string msg) {
//     cout << "[" << type << "] " << msg << '\n';
// }

// void print(std::string msg) {
//     cout << msg << '\n';
// }

void handle_new_connection(Kqueue& kq, TcpListener* server);

void handle_requests(Kqueue& event_queue,
                     TcpStream& client,
                     map<pair<string, string>, serverInfo>& infos);

HttpResponse handle_redirection(int status, string location) {
    if (status == 302)
        return HttpResponse::redirect_found_response(location);
    else if (status == 301)
        return HttpResponse::redirect_moved_response(location);
    cerr << "[ERROR] invalid status for "
            "redirection\n";
    exit(1);
}

/// args getting

std::vector<std::string> linespliting(std::string line) {
    size_t x;
    std::vector<std::string> data;
    size_t pos_start = 0;
    line += ";";
    while ((x = line.find(";", pos_start)) != std::string::npos) {
        data.push_back(line.substr(pos_start, x - pos_start));
        pos_start = x + 1;
    }
    return data;
}

std::map<std::string, std::string> grepdata(std::vector<std::string> data) {
    std::map<std::string, std::string> key_val;
    std::string trimed;
    size_t x;
    for (size_t i = 0; i < data.size(); i++) {
        trimed = tools::trim(data[i], " ");
        if (i == 0) {
            x = trimed.find(":");
            key_val[tools::trim(trimed.substr(0, x), " ")] =
                tools::trim(trimed.substr(x + 1, trimed.length() - x), " ");
            continue;
        }
        x = trimed.find("=");
        key_val[tools::trim(trimed.substr(0, x), " ")] =
            tools::trim(trimed.substr(x + 1, trimed.length() - x), " ");
    }
    return key_val;
}

std::map<std::string, std::string> get_fileinfo(std::string infos) {
    size_t x = 0;
    size_t pos_start = 0;
    std::map<std::string, std::string> ret;
    std::map<std::string, std::string> ret_;
    std::map<std::string, std::string>::iterator big;
    std::map<std::string, std::string>::iterator end;
    infos += "\r\n";
    while (1) {
        x = infos.find("\r\n", pos_start);
        if (x == std::string::npos)
            break;
        ret_ = grepdata(linespliting(infos.substr(pos_start, x - pos_start)));
        pos_start = x + 2;
        big = ret_.begin();
        end = ret_.end();
        while (big != end) {
            ret.insert(*big);
            big++;
        }
        if (pos_start >= infos.length())
            break;
    }
    return (ret);
}

std::string args_handling(std::string part) {
    size_t x = part.find("\r\n\r\n");
    if (x == std::string::npos)
        return "";
    std::map<std::string, std::string> fileinfo =
        get_fileinfo(part.substr(0, x));
    std::string key = trim(fileinfo["name"], "\"");
    std::string value = part.substr(x + 4, (part.length() - 2) - (x + 4));
    std::cout << "["
              << "$" + key + "=" + value << "]\n";
    return ("$" + key + "=" + value);
}

std::vector<std::string> extract_args(std::string body, std::string limit) {
    size_t y;
    string part;
    std::vector<std::string> res;
    size_t x = body.find(limit);
    if (x == std::string::npos)
        return res;
    size_t position = x + limit.length();
    while (1) {
        y = body.find(limit, position);  // end of text
        if (x == std::string::npos)
            break;
        part = body.substr(
            position + 2,
            y - 4 - position);  // + 4 for the /r/n.. in the end of file! and +
                                // 2 for /r/n in the end of line
        position = y + limit.length();
        part = args_handling(part);
        if (part == "")
            break;
        res.push_back(part);
    }
    return res;
}
// fileuploading

// std::vector<std::string> linespliting(std::string line) {
//     size_t x;
//     std::vector<std::string> data;
//     size_t pos_start = 0;
//     line += ";";
//     while ((x = line.find(";", pos_start)) != std::string::npos) {
//         data.push_back(line.substr(pos_start, x - pos_start));
//         pos_start = x + 1;
//     }
//     return data;
// }

// std::map<std::string, std::string> grepdata(std::vector<std::string> data) {
//     std::map<std::string, std::string> key_val;
//     std::string trimed;
//     size_t x;
//     for (size_t i = 0; i < data.size(); i++) {
//         trimed = tools::trim(data[i], " ");
//         if (i == 0) {
//             x = trimed.find(":");
//             key_val[tools::trim(trimed.substr(0, x), " ")] =
//                 tools::trim(trimed.substr(x + 1, trimed.length() - x), " ");
//             continue;
//         }
//         x = trimed.find("=");
//         key_val[tools::trim(trimed.substr(0, x), " ")] =
//             tools::trim(trimed.substr(x + 1, trimed.length() - x), " ");
//     }
//     return key_val;
// }

// std::map<std::string, std::string> get_fileinfo(std::string infos) {
//     size_t x = 0;
//     size_t pos_start = 0;
//     std::map<std::string, std::string> ret;
//     std::map<std::string, std::string> ret_;
//     std::map<std::string, std::string>::iterator big;
//     std::map<std::string, std::string>::iterator end;
//     infos += "\r\n";
//     while (1) {
//         x = infos.find("\r\n", pos_start);
//         if (x == std::string::npos)
//             break;
//         ret_ = grepdata(linespliting(infos.substr(pos_start, x -
//         pos_start))); pos_start = x + 2; big = ret_.begin(); end =
//         ret_.end(); while (big != end) {
//             ret.insert(*big);
//             big++;
//         }
//         if (pos_start >= infos.length())
//             break;
//     }
//     return (ret);
// }

int file_handling(std::string part, std::string location) {
    size_t x = part.find("\r\n\r\n");
    if (x == std::string::npos)
        return (1);
    std::map<std::string, std::string> fileinfo =
        get_fileinfo(part.substr(0, x));
    std::string filename =
        tools::url_path_correction(location, trim(fileinfo["filename"], "\""));
    std::ofstream outfile(filename.c_str());
    // size_t y = part.find("\r\n", x + 4);
    outfile << part.substr(x + 4, (part.length() - 2) - (x + 4));
    outfile.close();

    return (0);
}

int extract_files(std::string body, std::string limit, std::string location) {
    size_t y;
    string part;
    size_t x = body.find(limit);
    if (x == std::string::npos)
        return (1);
    size_t position = x + limit.length();
    while (1) {
        y = body.find(limit, position);  // end of text
        if (x == std::string::npos)
            break;
        part = body.substr(
            position + 2,
            y - 4 - position);  // + 4 for the /r/n.. in the end of file! and +
                                // 2 for /r/n in the end of line
        position = y + limit.length();
        if (file_handling(part, location))
            break;
    }
    return (0);
}

serverInfo get_the_server_info_for_the_client(
    string HostHeader,
    TcpStream& client,
    map<pair<string, string>, serverInfo>& server_infos) {
    // PARSE HOST HEADER FROM THE REQUEST
    // TODO handle empty Host
    assert(!HostHeader.empty());
    vector<string> tmp = split(HostHeader, ":");

    // TODO handle not having both the host and port in Host
    assert(tmp.size() <= 2);

    string host = trim(tmp.at(0), " ");
    string port;
    if (tmp.size() != 1)
        port = trim(tmp.at(1), " ");

    cout << "---> The request contains the Host header "
            "with {host, port} == {"
         << host << "," << port << "}" << '\n';

    if (client.get_port() != port) {
        port = client.get_port();
        host = HostHeader;
    }

    map<pair<string, string>, serverInfo>::iterator it =
        server_infos.find(make_pair(port, host));
    if (it == server_infos.end())
        return server_infos.at(make_pair(port, ""));
    return it->second;
}
// void printc(char c) {
//     printf("[%c] [%u] [%d]\n", c, c, c);
// }

pair<string, ssize_t> read_request(const TcpStream& client) {
    array<char, 1000 * 1000> buffer;
    ssize_t ret = 0;

    ret = client.read(buffer.data(), buffer.size());

    cout << G(DEBUG) << " return value of read is " << ret << '\n';
    //////
    std::string tmp;
    for (ssize_t s = 0; s < ret; s++) {
        tmp.push_back(buffer.data()[s]);
    }
    if (ret >= 0) {
        assert(static_cast<size_t>(ret) == tmp.size());
    }
    //////
    return make_pair(tmp, ret);
}

#include <csignal>
// #include <sys/signal.h>

// void handler(int sig) {
//     cout << G(ERROR) << " " << sig << " fuck tou pipe\n";
// }

int main() {
    signal(SIGPIPE, SIG_IGN);
#ifdef FAST
    cout.rdbuf(NULL);
    cerr.rdbuf(NULL);
#endif
    parser file("config");
    list<tokengen> tokens = file.generate();
    vector<serverInfo> servers_info = file.lexer_to_data(tokens);

    ////////////////////////////////////////////////////////////////////////
    Kqueue event_queue;

    set<pair<string, string> > already_bounded;
    map<pair<string, string>, serverInfo> infos;
    for (size_t i = 0; i < servers_info.size(); ++i) {
        for (size_t j = 0; j < servers_info[i].port.size(); ++j) {
            serverInfo& ser = servers_info[i];
            string& host = ser.host;
            string& port = ser.port[j];
            if (already_bounded.insert(make_pair(host, port)).second) {
                cout << G(INFO) << " binding and attaching {host, port} == {"
                     << host << "," << port << "}" << '\n';
                IListener* server = new TcpListener(host, port);
                event_queue.attach(server);
            }
        }
    }

    for (size_t i = 0; i < servers_info.size(); ++i) {
        for (size_t j = 0; j < servers_info[i].port.size(); ++j) {
            string port = servers_info[i].port[j];
            infos.insert(make_pair(make_pair(port, ""), servers_info[i]));
            for (size_t k = 0; k < servers_info[i].server_name.size(); ++k) {
                string host = servers_info[i].server_name[k];
                infos.insert(make_pair(make_pair(port, host), servers_info[i]));
            }
        }
    }
    std::cout << "-------------------------------------------------------------"
                 "------------------\n";
    loop {
        cout << G(INFO) << " Kqueue size == " << event_queue.size() << '\n';
        cout << G(INFO) << " waiting for events ....\n";
        IListener& listener = event_queue.get_event();
        cout << G(INFO) << " event fired\n";
        Kevent kv = listener.get_kevent();

        if (kv.filter == EVFILT_EXCEPT) {
            cout << G(INFO) << " EOF event\n";
            event_queue.detach(&listener);
            if (dynamic_cast<TcpListener*>(&listener)) {
                delete dynamic_cast<TcpListener*>(&listener);
            } else {
                delete dynamic_cast<TcpStream*>(&listener);
            }
            continue;
        }

        ssize_t ret = 0;
        cout << G(INFO) << " handling events\n";
        if (dynamic_cast<TcpListener*>(&listener)) {
            handle_new_connection(event_queue,
                                  dynamic_cast<TcpListener*>(&listener));
        } else {
            TcpStream& client = dynamic_cast<TcpStream&>(listener);
            cerr << G(DEBUG) << "request buffer size == "
                 << client.get_buffer_request().size() << '\n';
            cerr << G(DEBUG) << "response buffer size == "
                 << client.get_response_buffer().size() << '\n';

            if (kv.filter == EVFILT_WRITE) {
                // assert(client.is_response_not_finished() != 0);
                cerr << G(DEBUG) << " handling big write\n";
                ret = 0;
                std::string response = client.get_response_buffer();
                size_t towrite = 1000 * 1000;
                ;
                if (towrite > response.size())
                    towrite = response.size();
                if ((ret = client.write(response.data(), towrite)) <= 0) {
                    cerr << G(ERROR) << " " << strerror(errno) << '\n';
                    event_queue.detach(&client);
                    delete &client;
                    continue;
                }
                if (static_cast<size_t>(ret) <= response.size()) {
                    client.set_reponse_buffer(
                        response.substr(ret, response.size()));
                } else {
                    struct kevent evSet;
                    bzero(&evSet, sizeof(struct kevent));
                    EV_SET(&evSet, client.get_raw_fd(),
                           EVFILT_READ | EVFILT_WRITE | EVFILT_EXCEPT, EV_ADD,
                           0, 0, NULL);
                    if (kevent(event_queue.get_kdata(), &evSet, 1, NULL, 0,
                               NULL) == -1) {
                        std::cerr << "kevent is joking!\n";
                        exit(1);
                    }
                    client.set_reponse_buffer("");
                }
                continue;
            }
            cout << G(INFO) << " data == " << kv.data << endl;
            if (kv.data != 0) {
                cout << G(INFO) << " the client comming from {host, port} == {"
                     << client.get_host() << "," << client.get_port() << "}"
                     << '\n';
                cout << G(INFO) << " client x: " << client.get_raw_fd()
                     << "       ---> is ready for IO\n";
                cout << G(INFO) << " reading the request .." << endl;

                pair<string, ssize_t> p = read_request(client);
                cout << G(INFO) << " finished reading the request\n";
                const string& request_str = p.first;
                ret = p.second;
                cout << G(DEBUG) << " + " << G(DEBUG) << " return value is "
                     << ret << " size of the request is " << request_str.size()
                     << '\n';

                if (ret <= 0) {
                    if (ret < 0) {
                        cerr << G(ERROR) << " read error !\n";
                    }
                    event_queue.detach(&client);
                    delete &client;
                    continue;
                }
                client.add_to_request_buffer(request_str);
            }
            if (kv.data - ret == 0) {
                handle_requests(event_queue,
                                *dynamic_cast<TcpStream*>(&listener), infos);
            }
        }
    }
}

void handle_new_connection(Kqueue& event_queue, TcpListener* server) {
    TcpStream& client = server->accept();
    cout << G(INFO) << " the server with {host, port} == {"
         << server->get_host() << "," << server->get_port() << "}" << '\n';
    cout << "       ---> is accepting a new connection\n";
    cout << "----------------------------------------------------\n";
    cout << G(INFO) << " attaching the newly accepted client to the Kqueue\n";
    event_queue.attach(&client);
}

std::string get_response(HttpRequest request,
                         TcpStream& client,
                         map<pair<string, string>, serverInfo>& infos) {
    // TODO:
    //  HttpRequest request();
    if (request.error()) {
        cout << G(ERROR) << " failed to parse request!\n";

        return HttpResponse(403, "1.1", "Forbiden")
            .add_content_type(".html")
            .add_to_body("<h>404</h>")
            .build();
    }
    string HostHeader = request.getHeaderValue("Host");
    serverInfo info =
        get_the_server_info_for_the_client(HostHeader, client, infos);

    string root = info.root;
    string const& loc = request.getLocation();
    map<string, Location>& locations = info.locations;
    size_t _find;
    std::string _loc = loc;
    if ((_find = loc.find(".php")) != std::string::npos ||
        (_find = loc.find(".py")) != std::string::npos) {
        if (loc.length() - _find == 4)
            _loc = "*.php";
        if (loc.length() - _find == 3)
            _loc = "*.py";
    }
    map<string, Location>::const_iterator it = locations.find(_loc);
    const string& method = request.getMethod();
    cout << G(INFO) << " " << method << "\n";

    if (it == locations.end()) {
        if (method == "GET") {
            return HttpResponse::send_file(loc, info.root, info.error_page)
                .build();
        }
        return HttpResponse::error_response(405, info.error_page[405]).build();
    }
    Location route = it->second;
    std::cout << route.fastcgi_pass << "\n";
    if (route.fastcgi_pass != "") {
        // if (_loc == "*.py") {
        // }
        // else
        // {

        // }
        char c;
        std::string x = tools::url_path_correction(root, loc);
        std::string body;
        const char* path = route.fastcgi_pass.c_str();
        int fd[2];
        if (request.getMethod() == "POST" &&
            std::find(route.allow_methods.begin(), route.allow_methods.end(),
                      "POST") != route.allow_methods.end()) {
            pipe(fd);
            int pid = fork();
            if (pid < 0)
                assert(false);
            if (pid == 0) {
                cout << "REQUEST **********************\n";
                request.dump();
                cout << request.getBody() << '\n';
                cout << "REQUEST END**********************\n";
                setenv("REQUEST_METHOD", "POST", 1);
                setenv("CONTENT_LENGTH",
                       request.getHeaderValue("Content-Length").c_str(), 1);
                setenv("PATH_INFO", root.c_str(), 1);
                setenv("SCRIPT_FILENAME",
                       tools::url_path_correction(root, loc).c_str(), 1);
                setenv("REDIRECT_STATUS", "", 1);
                setenv("CONTENT_TYPE",
                       request.getHeaderValue("Content-Type").c_str(), 1);
                char* args[] = {(char*)path, (char*)x.c_str(), NULL};
                std::ofstream tmpfile("/tmp/Webcgi");
                tmpfile << tools::trim(request.getBody(), "\r\n\r\n");
                tmpfile.close();
                int fdf;
                fdf = open("/tmp/Webcgi", O_RDWR);
                dup2(fdf, 0);
                extern char** environ;
                char** env = environ;
                close(fdf);
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
                if (execve(path, args, env) == -1)
                    std::cerr << "ERROR EXECUTING CGI!\n";
                exit(1);
            }
            // wait(NULL);
        } else if (request.getMethod() == "GET" &&
                   std::find(route.allow_methods.begin(),
                             route.allow_methods.end(),
                             "GET") != route.allow_methods.end()) {
            pipe(fd);
            int pid = fork();
            if (pid < 0)
                assert(false);
            if (pid == 0) {
                setenv("REQUEST_METHOD", "GET", 1);
                setenv("PATH_INFO", root.c_str(), 1);
                setenv("SCRIPT_FILENAME",
                       tools::url_path_correction(root, loc).c_str(), 1);
                setenv("REDIRECT_STATUS", "", 1);
                char* args[] = {(char*)path, (char*)x.c_str(), NULL};
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
                execve(path, args, NULL);
                exit(1);
            }
        }
        wait(NULL);
        unlink("/tmp/Webcgi");
        close(fd[1]);
        while (read(fd[0], &c, 1) > 0) {
            body.push_back(c);
        }
        close(fd[0]);
        std::cout << "[" << body << "]"
                  << "\n";
        std::vector<std::string> the_hole = tools::split_(body, "\r\n\r\n");
        body = the_hole[the_hole.size() - 1];
        return HttpResponse(200, "1.1", "OK")
            .add_to_body(body)
            .add_content_type(".html")
            .build();
    } else if (request.getHeaderValue("Content-Length") != "") {
        if (request.getMethod() == "POST" && route.upload_enable) {
            if (find(route.allow_methods.begin(), route.allow_methods.end(),
                     method) == route.allow_methods.end()) {
                return HttpResponse::error_response(405, info.error_page[405])
                    .build();
            } else {
                if (request.getHeaderValue("Content-Type").empty()) {
                    return HttpResponse::error_response(400,
                                                        info.error_page[405])
                        .build();
                }
                vector<string> content_type =
                    split(request.getHeaderValue("Content-Type"), ";");
                if (content_type.size() != 2) {
                    cerr << "[ERROR] no boundry in content "
                            "type !!\n";
                    assert(false);
                } else {
                    string multi_part = content_type.at(0);
                    string boundry = content_type.at(1);
                    cout << "[INFO] " << multi_part << " " << boundry << '\n';
                    string boundry_value = split(boundry, "=").at(1);
                    cout << "[INFO] "
                         << "boundry key " << boundry << '\n';
                    if ("multipart/form-data" == multi_part)
                        extract_files(
                            request.getBody(), boundry_value,
                            tools::url_path_correction(root, it->first));
                    else {
                        assert(false);
                    }
                    return HttpResponse::redirect_moved_response("upload.html")
                        .build();
                }
            }
        }
    }

    if (find(route.allow_methods.begin(), route.allow_methods.end(), method) ==
        route.allow_methods.end()) {
        return HttpResponse::error_response(405, info.error_page[405]).build();
    }

    if (is_part_of_root(root, loc) &&
        is_dir(tools::url_path_correction(root, loc)) && route.autoindex) {
        return HttpResponse::generate_indexing(
                   tools::url_path_correction(root, loc), loc)
            .build();
    }

    if (method == "GET") {
        if (route.index.size() >= 1) {
            cout << G(DEBUG) << "handle indexes\n";
            return HttpResponse::index_response(route.index, info.root,
                                                info.error_page)
                .build();
        } else if (route.index.empty() && route.ret_rn.size() == 1) {
            assert(route.ret_rn.size() == 1);
            pair<int, string> redirect = *route.ret_rn.begin();
            cout << G(DEBUG) << "redirect " << redirect.first << " "
                 << redirect.second << '\n';

            return handle_redirection(redirect.first, redirect.second).build();
        }
    }
    if (method == "DELETE") {
        if (is_part_of_root(root, loc)) {
            char actualpath[PATH_MAX + 1];
            realpath(url_path_correction(root, loc).c_str(), actualpath);
            string cmd = "rm -rf " + string(actualpath);
            system(cmd.c_str());
            return HttpResponse(200, "1.1", "OK")
                .add_to_body("<h1>The file was deleted.</h1>")
                .add_content_type(".html")
                .build();
            ;
        }
        if (is_dir(url_path_correction(root, loc)) ||
            is_file(url_path_correction(root, loc)))
            return HttpResponse::error_response(405, info.error_page[405])
                .build();
        return HttpResponse::error_response(404, info.error_page[404]).build();
    }
    cerr << G(ERROR) << " no index + no return\n ";
    exit(1);
};

void handle_requests(Kqueue& event_queue,
                     TcpStream& client,
                     map<pair<string, string>, serverInfo>& infos) {
    // cout << G(DEBUG) << " request start\n";
    // cout << "[" << client.get_buffer_request() << "]" << '\n';
    // cout << G(DEBUG) << " request end\n";

    if (client.get_buffer_request().size() == 0) {
        cout << G(INFO) << " client detached from Kqueue\n";
        event_queue.detach(&client);
        delete &client;
        return;
    }
    // return;
    cout << G(INFO) << " parsing request..." << endl;
    HttpRequest request(client.get_buffer_request());

    cout << "ABC " << request.getHeaderValue("Content-Length") << '\n';
    cout << "ABC " << request.getBody().length() << '\n';
    // std::cout << "[" << request.getBody() << "]\n";
    // cout << G(INFO) << " " << request.getBody().length() << " "
    //      << std::stoul((request.getHeaderValue("Content-Length"))) << '\n';
    if (!request.error() && request.getHeaderValue("Content-Length") != "" &&
        request.getBody().length() + 4 <
            std::stoul((request.getHeaderValue("Content-Length")))) {
        return;
    }
    if (client.get_buffer_request().size() <= 1024)
        std::cout << client.get_buffer_request() << std::endl;
    client.clear_buffer();
    string response = get_response(request, client, infos);

    // std::ofstream outfile("test.txt");
    // outfile << client.get_buffer_request();
    // outfile.close();

    // cout << "[DEBUG] response start\n";
    // cout << response << '\n';
    // cout << "[DEBUG] response end\n";

    cout << G(DEBUG) << " size of buffer == " << response.size() << '\n';
    ssize_t ret = 0;
    size_t towrite = 1000 * 1000;

    // NOTE: this is called clamp
    if (towrite > response.size())
        towrite = response.size();

    // if (request.error()) {
    //     loop;
    // }
    cerr << G(INFO) << " responsing\n";
    if ((ret = client.write(response.data(), towrite)) <= 0) {
        if (ret < 0)
            cerr << G(ERROR) << " " << strerror(errno) << '\n';
        cout << G(INFO) << " client detached from Kqueue\n";
        event_queue.detach(&client);
        delete &client;
        return;
    }

    if (static_cast<size_t>(ret) < response.size()) {
        struct kevent evSet;
        bzero(&evSet, sizeof(struct kevent));
        EV_SET(&evSet, client.get_raw_fd(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        if (kevent(event_queue.get_kdata(), &evSet, 1, NULL, 0, NULL) == -1) {
            std::cerr << "kevent is joking!\n";
            exit(1);
        }
        client.set_reponse_buffer(response.substr(ret, response.size()));
        return;
    }
    cerr << G(DEBUG) << " return from send() == " << ret << '\n';
    if (request.error())
        return;
    if (request.getHeaderValue("Connection") == "" ||
        request.getHeaderValue("Connection") == "close") {
        cout << G(INFO) << " client detached from Kqueue\n";
        event_queue.detach(&client);
        delete &client;
    }
    cerr << G(DEBUG) << " end of handling" << ret << '\n';
}
