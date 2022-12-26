#include <stdlib.h>
#include <sys/_types/_pid_t.h>
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
#include "socket_header.hpp"
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
    vector<string> host_header = split_(HostHeader, ":");

    string host = trim(host_header[0], " ");
    string port;

    if (host_header.size() == 2) {
        port = trim(host_header[1], " ");
    }

    if (port.empty() || client.get_port() != port) {
        port = client.get_port();
        host = HostHeader;
    }

    map<pair<string, string>, serverInfo>::iterator it =
        server_infos.find(make_pair(port, host));
    if (it == server_infos.end())
        return server_infos.at(make_pair(port, ""));
    return it->second;
}

pair<string, ssize_t> read_request(const TcpStream& client) {
    array<char, BUFFER_SIZE> buffer;
    ssize_t ret = client.read(buffer.data(), buffer.size());
    if (ret <= 0)
        return make_pair(string(), ret);
    std::string tmp;
    tmp.reserve(ret);
    for (ssize_t s = 0; s < ret; s++) {
        tmp.push_back(buffer.data()[s]);
    }
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

    cout << G(INFO) << " parsing\n";
    parser file("config");
    list<tokengen> tokens = file.generate();
    vector<serverInfo> servers_info = file.lexer_to_data(tokens);
    ////////////////////////////////////////////////////////////////////////

    cout << G(INFO) << " creating the servers\n";
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

    ////////////////////////////////////////////////////////////////////////

    cout << G(INFO) << " start the main loop\n";
    loop {
        cout << G(INFO) << " waiting for events ....\n";
        IListener& listener = event_queue.get_event();
        cout << G(INFO) << " event fired\n";
        Kevent kv = listener.get_kevent();

        if (kv.filter == EVFILT_EXCEPT) {
            event_queue.detach(&listener);
            if (dynamic_cast<TcpListener*>(&listener)) {
                delete dynamic_cast<TcpListener*>(&listener);
            } else {
                delete dynamic_cast<TcpStream*>(&listener);
            }
            continue;
        }

        ssize_t ret = 0;
        if (dynamic_cast<TcpListener*>(&listener)) {
            handle_new_connection(event_queue,
                                  dynamic_cast<TcpListener*>(&listener));
        } else {
            TcpStream& client = dynamic_cast<TcpStream&>(listener);

            if (kv.filter == EVFILT_WRITE) {
                cerr << G(INFO) << " handling big write\n";
                std::string response = client.get_response_buffer();
                size_t towrite = BUFFER_SIZE;
                if (towrite > response.size())
                    towrite = response.size();
                if ((ret = client.write(response.data(), towrite)) <= 0) {
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
                        event_queue.detach(&client);
                        delete &client;
                        continue;
                    }
                    client.set_reponse_buffer("");
                }
                continue;
            }
            if (kv.filter & EVFILT_READ && kv.data == 0) {
                event_queue.detach(&client);
                delete &client;
                continue;
            }
            if (kv.data != 0) {
                cout << G(INFO) << " client is ready for IO " << endl;
                pair<string, ssize_t> p = read_request(client);
                ret = p.second;
                if (ret <= 0) {
                    event_queue.detach(&client);
                    delete &client;
                    continue;
                }
                const string& request_str = p.first;
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
    cout << G(INFO) << "accepting a new connection\n";
    event_queue.attach(&client);
}

std::string get_response(HttpRequest request,
                         TcpStream& client,
                         map<pair<string, string>, serverInfo>& infos) {
    if (request.error()) {
        return HttpResponse::error_response(403, "").build(request);
    } else if (request.getVersion() != "HTTP/1.1") {
        return HttpResponse::error_response(505, "").build(request);
    }
    string HostHeader = request.getHeaderValue("Host");
    serverInfo info =
        get_the_server_info_for_the_client(HostHeader, client, infos);

    string const& root = info.root;
    string const& loc = request.getLocation();
    map<string, Location>& locations = info.locations;
    size_t _find;
    std::string _loc = loc;
    if ((_find = loc.find(".php")) != std::string::npos ||
        (_find = loc.find(".py")) != std::string::npos) {
        if (loc.length() - _find == 4)
            _loc = "*.php";
        else if (loc.length() - _find == 3)
            _loc = "*.py";
    }
    map<string, Location>::const_iterator it = locations.find(_loc);
    const string& method = request.getMethod();

    if (it == locations.end()) {
        if (method == "GET") {
            return HttpResponse::send_file(loc, info.root,
                                           info.error_page)  /// cout
                .build(request);
        }
        return HttpResponse::error_response(405, info.error_page[405])
            .build(request);
    }
    Location route = it->second;
    if (route.fastcgi_pass != "") {
        char c;
        std::string script_path = tools::url_path_correction(root, loc);
        std::string body;
        const char* cgi_path = route.fastcgi_pass.c_str();
        int fd[2];
        std::string tmpfile_name = tools::generateRandomTempFileName();
        if (request.getMethod() == "POST" &&
            std::find(route.allow_methods.begin(), route.allow_methods.end(),
                      "POST") != route.allow_methods.end()) {
            if ((info.client_max_body_size * 1024) <
                std::stoull(request.getHeaderValue("Content-Length"))) {
                return HttpResponse::error_response(413, info.error_page[413])
                    .build();
            }
            if (pipe(fd) < 0) {
                return HttpResponse::error_response(503, info.error_page[503])
                    .build();
            }

            int pid = fork();
            if (pid < 0) {
                return HttpResponse::error_response(503, info.error_page[503])
                    .build();
            }
            if (pid == 0) {
                if (!request.getHeaderValue("Cookie").empty())
                    setenv("HTTP_COOKIE",
                           request.getHeaderValue("Cookie").c_str(), 1);
                setenv("REQUEST_METHOD", "POST", 1);
                setenv("CONTENT_LENGTH",
                       request.getHeaderValue("Content-Length").c_str(), 1);
                setenv("PATH_INFO", root.c_str(), 1);
                setenv("SCRIPT_FILENAME",
                       tools::url_path_correction(root, loc).c_str(), 1);
                setenv("REDIRECT_STATUS", "", 1);
                setenv("CONTENT_TYPE",
                       request.getHeaderValue("Content-Type").c_str(), 1);
                char* args[] = {(char*)cgi_path, (char*)script_path.c_str(),
                                NULL};
                std::ofstream tmpfile(tmpfile_name);
                if (!tmpfile.is_open() || tmpfile.fail()) {
                    exit(1);
                }
                tmpfile << tools::trim(request.getBody(), "\r\n\r\n");
                tmpfile.close();
                int fdf;
                fdf = open(tmpfile_name.c_str(), O_RDWR);
                dup2(fdf, 0);
                extern char** environ;
                char** env = environ;
                close(fdf);
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
                if (execve(cgi_path, args, env) == -1)
                    std::cerr << "ERROR EXECUTING CGI!\n";
                exit(1);
            }
        } else if (request.getMethod() == "GET" &&
                   std::find(route.allow_methods.begin(),
                             route.allow_methods.end(),
                             "GET") != route.allow_methods.end()) {
            if (pipe(fd) < 0) {
                return HttpResponse::error_response(503, info.error_page[503])
                    .build();
            }
            int pid = fork();
            if (pid < 0) {
                return HttpResponse::error_response(503, info.error_page[503])
                    .build();
            }
            if (pid == 0) {
                if (request.getHeaderValue("Cookie") != "")
                    setenv("HTTP_COOKIE",
                           request.getHeaderValue("Cookie").c_str(), 1);
                setenv("REQUEST_METHOD", "GET", 1);
                setenv("PATH_INFO", root.c_str(), 1);
                setenv("SCRIPT_FILENAME",
                       tools::url_path_correction(root, loc).c_str(), 1);
                setenv("REDIRECT_STATUS", "", 1);
                extern char** environ;
                char** env = environ;
                char* args[] = {(char*)cgi_path, (char*)script_path.c_str(),
                                NULL};
                close(fd[0]);
                dup2(fd[1], 1);
                close(fd[1]);
                execve(cgi_path, args, env);
                exit(1);
            }
        }
        close(fd[1]);
        unlink(tmpfile_name.c_str());
        pid_t error_status = 0;
        wait(&error_status);
        if (error_status != 0) {
            close(fd[0]);
            return HttpResponse::error_response(409, info.error_page[409])
                .build();
        }
        while (read(fd[0], &c, 1) > 0) {
            body.push_back(c);
        }
        close(fd[0]);
        std::vector<std::string> cgi_result = tools::split_(body, "\r\n\r\n");
        if (cgi_result.size() != 2) {
            return HttpResponse::error_response(502, info.error_page[502])
                .build();
        }
        body = cgi_result[cgi_result.size() - 1];

        // parse additional headers:
        HttpResponse http_response =
            HttpResponse(200, "1.1", "OK").add_to_body(body);
        std::vector<string> headers = split(cgi_result[0], "\n");
        for (size_t i = 0; i < headers.size(); ++i) {
            std::vector<string> key_header = split_(headers[i], ":");
            if (key_header.size() == 1)
                http_response.add_header(key_header[0], "");
            else
                http_response.add_header(key_header[0], key_header[1]);
        }

        return http_response.build(request);
    } else if (request.getHeaderValue("Content-Length") != "") {
        if (request.getMethod() == "POST" && route.upload_enable) {
            if ((info.client_max_body_size * 1024) <
                std::stoull(request.getHeaderValue("Content-Length"))) {
                return HttpResponse::error_response(413, info.error_page[413])
                    .build();
            }
            if (find(route.allow_methods.begin(), route.allow_methods.end(),
                     method) == route.allow_methods.end()) {
                return HttpResponse::error_response(405, info.error_page[405])
                    .build(request);
            } else {
                if (request.getHeaderValue("Content-Type").empty()) {
                    return HttpResponse::error_response(400,
                                                        info.error_page[400])
                        .build(request);
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
                        .build(request);
                }
            }
        }
    }

    if (find(route.allow_methods.begin(), route.allow_methods.end(), method) ==
        route.allow_methods.end()) {
        return HttpResponse::error_response(405, info.error_page[405])
            .build(request);
    }

    if (is_part_of_root(root, loc) &&
        is_dir(tools::url_path_correction(root, loc)) && route.autoindex) {
        return HttpResponse::generate_indexing(
                   tools::url_path_correction(root, loc), loc)
            .build(request);
    }

    if (method == "GET") {
        if (route.index.size() >= 1) {
            cout << G(DEBUG) << "handle indexes\n";
            return HttpResponse::index_response(route.index, info.root,
                                                info.error_page)
                .build(request);
        } else if (route.index.empty() && route.ret_rn.size() == 1) {
            assert(route.ret_rn.size() == 1);
            pair<int, string> redirect = *route.ret_rn.begin();
            cout << G(DEBUG) << "redirect " << redirect.first << " "
                 << redirect.second << '\n';

            return handle_redirection(redirect.first, redirect.second)
                .build(request);
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
                .build(request);
            ;
        }
        if (is_dir(url_path_correction(root, loc)) ||
            is_file(url_path_correction(root, loc)))
            return HttpResponse::error_response(405, info.error_page[405])
                .build(request);
        return HttpResponse::error_response(404, info.error_page[404])
            .build(request);
    }
    cerr << G(ERROR) << " no index + no return\n ";
    exit(1);
};

void handle_requests(Kqueue& event_queue,
                     TcpStream& client,
                     map<pair<string, string>, serverInfo>& infos) {
    if (client.get_buffer_request().size() == 0) {
        event_queue.detach(&client);
        delete &client;
        return;
    }
    HttpRequest request(client.get_buffer_request());

    if (!request.error() && request.getMethod() == "POST" &&
        request.getHeaderValue("Content-Length") != "" &&
        request.getBody().length() <
            std::stoull((request.getHeaderValue("Content-Length")))) {
        return;
    }
    client.clear_buffer();
    string response = get_response(request, client, infos);

    ssize_t ret = 0;
    size_t towrite = BUFFER_SIZE;

    if (towrite > response.size())
        towrite = response.size();
    if ((ret = client.write(response.data(), towrite)) <= 0) {
        event_queue.detach(&client);
        delete &client;
        return;
    }

    if (static_cast<size_t>(ret) < response.size()) {
        struct kevent evSet;
        bzero(&evSet, sizeof(struct kevent));
        EV_SET(&evSet, client.get_raw_fd(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
        if (kevent(event_queue.get_kdata(), &evSet, 1, NULL, 0, NULL) == -1) {
            event_queue.detach(&client);
            delete &client;
            return;
        }
        client.set_reponse_buffer(response.substr(ret, response.size()));
        return;
    }

    if (request.error() || request.getHeaderValue("Connection").empty() ||
        request.getHeaderValue("Connection") == "close") {
        event_queue.detach(&client);
        delete &client;
    }
}
