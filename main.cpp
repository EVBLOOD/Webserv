#include <algorithm>
#include <cstdio>
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

void handle_new_connection(Kqueue& kq,
                           TcpListener* server,
                           map<pair<string, string>, serverInfo>& infos);

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
    array<char, 1024> buffer;
    ssize_t ret = 0;

    ret = client.read(buffer.data(), buffer.size());

    cout << G(DEBUG) << " return value of read is " << ret << '\n';
    return make_pair(std::string(buffer.data()), ret);
}

int main() {
#ifdef FAST
    cout.rdbuf(NULL);
    cerr.rdbuf(NULL);
#endif
    parser file("conf");
    list<tokengen> tokens = file.generate();
    vector<serverInfo> servers_info = file.lexer_to_data(tokens);
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
    loop {
        cout << G(INFO) << " waiting for events ....\n";
        IListener& listener = event_queue.get_event();
        Kevent kv = listener.get_kevent();

        if (kv.flags & EV_EOF) {
            event_queue.detach(&listener);
            if (dynamic_cast<TcpListener*>(&listener)) {
                delete dynamic_cast<TcpListener*>(&listener);
            } else {
                delete dynamic_cast<TcpStream*>(&listener);
            }
            continue;
        }
        IF_NOT(kv.flags & (EVFILT_READ | EVFILT_WRITE)) continue;

        ssize_t ret = 0;
        cout << G(INFO) << " handling events\n";
        if (dynamic_cast<TcpListener*>(&listener)) {
            handle_new_connection(event_queue,
                                  dynamic_cast<TcpListener*>(&listener), infos);
        } else {
            TcpStream& client = dynamic_cast<TcpStream&>(listener);
            cout << G(INFO) << " data == " << kv.data << endl;
            if (kv.data != 0) {
                cout << G(INFO) << " the client comming from {host, port} == {"
                     << client.get_host() << "," << client.get_port() << "}"
                     << '\n';
                cout << "       ---> is ready for IO\n";
                cout << G(INFO) << " reading the request .." << endl;

                pair<string, ssize_t> p = read_request(client);
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
                } else {
                    client.add_to_buffer(request_str);
                }
            }
            if (kv.data - ret == 0) {
                handle_requests(event_queue,
                                *dynamic_cast<TcpStream*>(&listener), infos);
            }
        }
    }
}

void handle_new_connection(Kqueue& event_queue,
                           TcpListener* server,
                           map<pair<string, string>, serverInfo>& infos) {
    TcpStream& client = server->accept();
    cout << G(INFO) << " the server with {host, port} == {"
         << server->get_host() << "," << server->get_port() << "}" << '\n';
    cout << "       ---> is accepting a new connection\n";
    cout << "----------------------------------------------------\n";
    cout << G(INFO) << " attaching the newly accepted client to the Kqueue\n";
    event_queue.attach(&client);
}

void handle_requests(Kqueue& event_queue,
                     TcpStream& client,
                     map<pair<string, string>, serverInfo>& infos) {
    cout << G(DEBUG) << " request start\n";
    cout << "[" << client.get_buffer() << "]" << '\n';
    cout << G(DEBUG) << " request end\n";

    cout << G(INFO) << " parsing the request started " << endl;

    string response;
    HttpRequest request(client.get_buffer());

    if (request.error()) {
        cout << G(ERROR) << " failed to parse request(not HTTP 1.1)\n";

        response = HttpResponse(403, "1.1", "Forbiden")
                       .add_content_type(".html")
                       .add_to_body("<h>404</h>")
                       .build();
    } else {
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
                response =
                    HttpResponse::send_file(loc, info.root, info.error_page)
                        .build();
            } else {
                response =
                    HttpResponse::error_response(405, info.error_page[405])
                        .build();
            }
        } else {
            Location route = it->second;
            if (request.getHeaderValue("Content-Length") != "") {
                if (request.getMethod() == "POST" && route.upload_enable) {
                    if (find(route.allow_methods.begin(),
                             route.allow_methods.end(),
                             method) == route.allow_methods.end()) {
                        response = HttpResponse::error_response(
                                       405, info.error_page[405])
                                       .build();
                    } else {
                        if (request.getHeaderValue("Content-Type").empty()) {
                            response = HttpResponse::error_response(
                                           400, info.error_page[405])
                                           .build();
                        } else {
                            vector<string> content_type = split(
                                request.getHeaderValue("Content-Type"), ";");
                            if (content_type.size() != 2) {
                                cerr << "[ERROR] no boundry in content "
                                        "type !!\n";
                                assert(false);
                            } else {
                                // TODO SAAD
                                string multi_part = content_type.at(0);
                                string boundry = content_type.at(1);
                                cout << "[INFO] " << multi_part << " "
                                     << boundry << '\n';
                                string boundry_value =
                                    split(boundry, "=").at(1);
                                cout << "[INFO] "
                                     << "boundry key " << boundry << '\n';
                                {
                                    // TODO check if the body have the
                                    // needed info
                                }
                                response =
                                    HttpResponse::redirect_moved_response(
                                        "upload.html")
                                        .build();
                                // SAAD
                            }
                        }
                        return;
                    }
                }
            } else {
                if (find(route.allow_methods.begin(), route.allow_methods.end(),
                         method) == route.allow_methods.end()) {
                    response =
                        HttpResponse::error_response(405, info.error_page[405])
                            .build();

                } else {
                    if (is_part_of_root(root, loc) &&
                        is_dir(tools::url_path_correction(root, loc)) &&
                        route.autoindex) {
                        response =
                            HttpResponse::generate_indexing(
                                tools::url_path_correction(root, loc), loc)
                                .build();
                    } else {
                        if (route.index.size() >= 1) {
                            cout << G(DEBUG) << "handle indexes\n";
                            response =
                                HttpResponse::index_response(
                                    route.index, info.root, info.error_page)
                                    .build();
                        } else if (route.index.empty() &&
                                   route.ret_rn.size() == 1) {
                            assert(route.ret_rn.size() == 1);
                            pair<int, string> redirect = *route.ret_rn.begin();
                            cout << G(DEBUG) << " redirect " << redirect.first
                                 << " " << redirect.second << '\n';

                            response = handle_redirection(redirect.first,
                                                          redirect.second)
                                           .build();
                        } else {
                            cerr << G(ERROR) << " no index + no return \n";
                            exit(1);
                        }
                    }
                }
            }
        }
    }
    client.clear_buffer();
    cout << "[DEBUG] response start\n";
    cout << response << '\n';
    cout << "[DEBUG] response end\n";

    client.write(response.data(), response.size());
    if (request.error() || request.getHeaderValue("Connection") == "" ||
        request.getHeaderValue("Connection") == "close") {
        cout << G(INFO) << " client detached from Kqueue\n";
        event_queue.detach(&client);
        delete &client;
        return;
    }
}
