#include <iostream>
#include <set>
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
using namespace tools;
using std::cerr;
using std::cout;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

void handle_new_connection(Kqueue& kq, TcpListener* server);
void handle_requests(Kqueue& event_queue,
                     TcpStream& client,
                     map<pair<string, string>, serverInfo> infos);

HttpResponse handle_redirection(int status, std::string location) {
    if (status == 302)
        return HttpResponse::redirect_found_response(location);
    else if (status == 301)
        return HttpResponse::redirect_moved_response(location);
    cerr << "[ERROR] invalid status for "
            "redirection\n";
    exit(1);
}

serverInfo get_the_server_info_for_the_client(
    std::string HostHeader,
    TcpStream& client,
    map<pair<string, string>, serverInfo>& server_infos) {
    // PARSE HOST HEADER FROM THE REQUEST
    // TODO handle empty Host
    assert(!HostHeader.empty());
    vector<string> tmp = split(HostHeader, ":");

    // TODO handle not having both the host and port in Host
    assert(tmp.size() <= 2);

    std::string host = trim(tmp.at(0), " ");
    std::string port;
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
        server_infos.find(std::make_pair(port, host));
    if (it == server_infos.end())
        return server_infos.at(make_pair(port, ""));
    return it->second;
}

int main() {
    parser file("conf");
    std::list<tokengen> tokens = file.generate();
    std::vector<serverInfo> servers_info = file.lexer_to_data(tokens);

    Kqueue event_queue;

    set<pair<string, string> > already_bounded;
    map<pair<string, string>, serverInfo> infos;
    for (size_t i = 0; i < servers_info.size(); ++i) {
        for (size_t j = 0; j < servers_info[i].port.size(); ++j) {
            serverInfo& ser = servers_info[i];
            std::string& host = ser.host;
            std::string& port = ser.port[j];
            if (already_bounded.insert(make_pair(host, port)).second) {
                cout << "[INFO] binding and attaching {host, port} == {" << host
                     << "," << port << "}" << '\n';
                event_queue.attach(new TcpListener(host, port));
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
        cout << "[INFO] waiting for events ....\n";
        std::vector<IListener*> events = event_queue.get_events();
        cout << "[INFO] handling events\n";
        cout << "       ---> number of events ready for IO : " << events.size()
             << '\n';
        while (events.size()) {
            IListener* event = events.back();
            if (dynamic_cast<TcpListener*>(event)) {
                handle_new_connection(event_queue,
                                      dynamic_cast<TcpListener*>(event));
            } else {
                handle_requests(event_queue, *dynamic_cast<TcpStream*>(event),
                                infos);
            }
            events.pop_back();
        }
    }
}

void handle_new_connection(Kqueue& kq, TcpListener* server) {
    TcpStream& client = server->accept();
    cout << "----------------------------------------------------\n";
    cout << "[INFO] the server with {host, port} == {" << server->get_host()
         << "," << server->get_port() << "}" << '\n';
    cout << "       ---> is accepting a new connection\n";
    cout << "[INFO] attaching the newly accepted client\n";
    cout << "----------------------------------------------------\n";
    kq.attach(&client);
}

void handle_requests(Kqueue& event_queue,
                     TcpStream& client,
                     map<pair<string, string>, serverInfo> infos) {
    cout << "----------------------------------------------------\n";
    cout << "[INFO] the client comming from {host, port} == {"
         << client.get_host() << "," << client.get_port() << "}" << '\n';
    cout << "       ---> is ready for IO\n";
    cout << "[INFO] reading the request ..." << std::endl;
    std::array<char, 4096> buffer;
    std::string request_str;
    ssize_t ret = 0;
    loop {
        buffer.fill(0);
        if ((ret = client.read(buffer.data(), buffer.size()) <= 0)) {
            break;
        }
        request_str += std::string(buffer.data());
        cout << "[DEBUG] return value of read is " << ret
             << " size of the request is " << request_str.size() << '\n';
        if (request_str.size() > 2 &&
            request_str.substr(request_str.size() - 2, request_str.size()) ==
                "\r\n") {
            break;
        }
        cout << "[INFO] reading the request ..." << std::endl;
    }
    cout << "[DEBUG] return value is " << ret << " size of the request is "
         << request_str.size() << '\n';
    if ((request_str.size() == 0 && ret == 0) || ret < 0) {
        if (ret < 0) {
            cerr << "[ERRRO] read error\n";
        }
        event_queue.detach(&client);
        client.shutdown();
    } else {
        cout << "[DEBUG] request start\n";
        cout << buffer.data();
        cout << "[DEBUG] request end\n";

        cout << "[INFO] parsing the request" << std::endl;
        HttpRequest request = HttpRequest(buffer.data());
        {
            if (request.getHeaderValue("Content-Length") != "") {
                cerr << "[TODO] handling request with a body -- checking "
                        "if the body size is equal to Content-Length\n";
                assert(false);
            }
        }

        std::string HostHeader = request.getHeaderValue("Host");
        serverInfo info =
            get_the_server_info_for_the_client(HostHeader, client, infos);

        // TODO ////////////////////////////////////////////////////
        // then responde with the proper thing after checking if  //
        // the methode is allowed using the Location class        //
        ////////////////////////////////////////////////////////////

        std::string response;
        std::string root = info.root;
        std::string const& loc = request.getLocation();
        map<string, Location>& locations = info.locations;
        map<string, Location>::const_iterator it = locations.find(loc);

        if (it == locations.end()) {
            response = HttpResponse::send_file(loc, info.root, info.error_page)
                           .build();
        } else {
            Location route = it->second;
            const std::string& method = request.getMethod();

            if (is_part_of_root(root, loc) &&
                is_dir(tools::url_path_correction(root, loc)) &&
                route.autoindex) {
                response = HttpResponse::generate_indexing(
                               tools::url_path_correction(root, loc), loc)
                               .build();
            } else {
                if (route.index.size() >= 1) {
                    cerr << "[DEBUG] handle indexes\n";
                    response = HttpResponse::index_response(
                                   route.index, info.root, info.error_page)
                                   .build();
                } else if (route.index.empty() && route.ret_rn.size() == 1) {
                    assert(route.ret_rn.size() == 1);
                    pair<int, string> ret = *route.ret_rn.begin();
                    cout << "[DEBUG] redirect " << ret.first << " "
                         << ret.second << '\n';

                    response =
                        handle_redirection(ret.first, ret.second).build();
                } else {
                    cerr << "[ERROR] no index + no return \n";
                    exit(1);
                }
            }
        }
        // cout << "[DEBUG] response start\n";
        // cout << response << '\n';
        // cout << "[DEBUG] response end\n";
        client.write(response.data(), response.size());
    }
}
