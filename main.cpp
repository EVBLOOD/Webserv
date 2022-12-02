#include <set>
#include "New/TcpStream.hpp"
#include "New/kqueue.hpp"
#include "New/tcpListener.hpp"
#include "Server.hpp"
#include "ServerBuilder.hpp"
#include "ServerPoll.hpp"
#include "parsing/location.hpp"
#include "parsing/parser.hpp"
#include "parsing/tokengen.hpp"

#define loop for (;;)
using std::cerr;
using std::cout;
using std::map;
using std::pair;
using std::set;
using std::string;
using std::vector;

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
    ///// PARSING THE CONFIG FILE
    parser file("conf");
    std::list<tokengen> tokens = file.generate();
    std::vector<serverInfo> servers_info = file.lexer_to_data(tokens);
    /////////////////////////////
    ///// CREATING KQUEUE
    Kqueue event_queue;
    /////////////////////////////
    ///// LOOPING OVER THE PORTS
    ///// AND HOST AND CREATING TCP LISTNERS ("SERVERS")
    ///// AND KEEPING TRACK OF THE BOUNDED PORT/HOST COMBO
    set<pair<string, string> > already_bounded;
    ///// CREATE MAP KEY IS A PAIR OF PORT AND HOST AND VALUE IS SERVER INFO
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
    // for (const auto& info : infos) {
    //     cout << info.first.first << " [" << info.first.second << "] "
    //          << info.second.root << '\n';
    // }
    ///// PREPARE ROUTES
    // map<pair<serverInfo, std::string>, location> routes;
    // for (size_t i = 0; i < servers_info.size(); ++i) {
    //     for (size_t j = 0; j < servers_info[i].locations.size(); ++j) {
    //         location& loc = servers_info[i].locations[j];
    //         routes[make_pair(servers_info[i], )]
    //     }
    // }
    /////////////////////////////////////////////
    /// CREATING EVENT LOOP
    loop {
        // KQUEUE RETURNS ALL LISTNERS READY FOR
        cout << "[INFO] waiting for events ....\n";
        std::vector<IListener*> events = event_queue.get_events();
        cout << "[INFO] handling events\n";
        cout << "       ---> number of events ready for IO : " << events.size()
             << '\n';
        while (events.size()) {
            IListener* event = events.back();
            if (dynamic_cast<IServer*>(event)) {
                TcpListener* server = dynamic_cast<TcpListener*>(event);
                IStreamer& client = server->accept();
                cout
                    << "----------------------------------------------------\n";
                cout << "[INFO] the server with {host, port} == {"
                     << server->get_host() << "," << server->get_port() << "}"
                     << '\n';
                cout << "       ---> is accepting a new connection\n";
                cout << "[INFO] attaching the newly accepted client\n";
                cout
                    << "----------------------------------------------------\n";
                event_queue.attach(&client);
            } else {
                TcpStream& client = *dynamic_cast<TcpStream*>(event);
                cout
                    << "----------------------------------------------------\n";
                cout << "[INFO] the client comming from {host, port} == {"
                     << client.get_host() << "," << client.get_port() << "}"
                     << '\n';
                cout << "       ---> is ready for IO\n";
                cout << "[INFO] reading the request" << std::endl;
                std::array<char, 1024> buffer;
                buffer.fill(0);
                if (client.read(buffer.data(), buffer.size()) <= 0) {
                    event_queue.detach(&client);
                    client.shutdown();
                }

                cout << "[DEBUG] request start\n";
                cout << buffer.data();
                cout << "[DEBUG] request end\n";

                /////////////////////////////////////////
                {
                    cout << "[INFO] parsing the request" << std::endl;
                    // PARSING REQUEST
                    HttpRequest request = HttpRequest(buffer.data());
                    //////////////////
                    // PARSE HOST HEADER FROM THE REQUEST
                    std::string HostHeader = request.getHeaderValue("Host");
                    {
                        serverInfo info = get_the_server_info_for_the_client(
                            HostHeader, client, infos);

                        // TODO handle request with the server info
                        // check if auto indexing is on and display the dir if
                        // the toute isnt a file return a file if possible
                        // ------ persmission denied should return out default
                        // error page if the page isnt set the conf file ------
                        // ------ all error should behave similarly -------
                        // the route should start with the root
                        // then responde with the proper thing after checking if
                        // the methode is allowed using the Location class
                        /////////////////////////////////////////

                        std::string response;
                        std::string const& loc = request.getLocation();
                        map<string, Location>& locations = info.locations;
                        map<string, Location>::const_iterator it =
                            locations.find(loc);
                        if (it == locations.end()) {
                            response = HttpResponse::send_file(loc, info.root,
                                                               info.error_page)
                                           .build();
                            client.write(response.c_str(), response.size());
                            events.pop_back();
                            continue;
                        }
                        ////// if Route is in Locations
                        Location route = it->second;
                        const std::string& method = request.getMethod();
                        if (find(route.allow_methods.begin(),
                                 route.allow_methods.end(),
                                 method) == route.allow_methods.end()) {
                            // TODO handle a no allowed method
                            cerr << "[TODO] method is not allowed\n";
                            assert(false);
                        } else {
                            if (route.autoindex) {
                                // TODO handle auto index on
                            } else {
                                {
                                    vector<string>::iterator it =
                                        route.index.begin();
                                    while (it != route.index.end()) {
                                        cout << "[DEBUG] " << *it << '\n';
                                        ++it;
                                    }
                                }
                                if (route.index.size() == 1) {
                                    cout << "[DEBUG] full path "
                                         << info.root + route.index[0] << '\n';
                                    std::ifstream file(info.root + "/" +
                                                       route.index[0]);
                                    response =
                                        HttpResponse(200, "1.1", "OK")
                                            .add_to_body(open_to_serve(file))
                                            .add_header("Content-Type",
                                                        "text/html")
                                            .build();
                                } else if (route.index.size() > 1) {
                                    // TODO handle multiple indexes
                                    cerr << "[TODO] multiple indexes\n";
                                    assert(false);
                                } else {
                                    map<int, string>::iterator it =
                                        route.ret_rn.begin();
                                    while (it != route.ret_rn.end()) {
                                        cout << "[DEBUG] " << it->first << " "
                                             << it->second << '\n';
                                        ++it;
                                    }
                                    cerr << "[TODO] no index\n";
                                    assert(false);
                                }
                            }
                        }
                        client.write(response.data(), response.size());
                    }
                }
            }
            events.pop_back();
        }
    }
    ////////////////////////
}
