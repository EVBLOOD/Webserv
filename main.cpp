#include <sys/event.h>
#include <sys/signal.h>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <set>
#include <string>
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
    map<string, Location>::const_iterator it = locations.find(loc);
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
    if (request.getHeaderValue("Content-Length") != "") {
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
                    // TODO SAAD
                    string multi_part = content_type.at(0);
                    string boundry = content_type.at(1);
                    cout << "[INFO] " << multi_part << " " << boundry << '\n';
                    string boundry_value = split(boundry, "=").at(1);
                    cout << "[INFO] "
                         << "boundry key " << boundry << '\n';
                    {
                        // TODO check if the body have the
                        // needed info
                    }
                    return HttpResponse::redirect_moved_response("upload.html")
                        .build();
                    // SAAD
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
    // cout << G(INFO) << " " << request.getBody().length() << " "
    //      << std::stoul((request.getHeaderValue("Content-Length"))) << '\n';
    if (!request.error() && request.getHeaderValue("Content-Length") != "" &&
        request.getBody().length() + 4 <
            std::stoul((request.getHeaderValue("Content-Length")))) {
        return;
    }

    string response = get_response(request, client, infos);

    client.clear_buffer();
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
