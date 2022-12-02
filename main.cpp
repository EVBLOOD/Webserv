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
                cout << "[INFO] the server with {host, port} == {"
                     << server->get_host() << "," << server->get_port() << "}"
                     << '\n';
                cout << "       ---> is accepting a new connection\n";
                cout << "[INFO] attaching the newly accepted client\n";
                event_queue.attach(&client);
            } else {
                TcpStream& client = *dynamic_cast<TcpStream*>(event);
                cout << "[INFO] the client comming from {host, port} == {"
                     << client.get_host() << "," << client.get_port() << "}"
                     << '\n';
                cout << "       ---> is ready for IO\n";
                std::array<char, 4096> buffer;
                buffer.fill(0);
                client.read(buffer.data(), buffer.size());
                cout << "[INFO] reading the request\n";

                /////////////////////////////////////////
                /// TODO
                {
                    // PARSING REQUEST
                    HttpRequest request = HttpRequest(buffer.data());
                    //////////////////
                    // PARSE HOST HEADER FROM THE REQUEST
                    std::string HostHeader = request.getHeaderValue("Host");
                    {
                        assert(!HostHeader.empty());
                        vector<string> tmp = split(HostHeader, ":");

                        assert(tmp.size() <= 2);

                        std::string host = trim(tmp.at(0), " ");
                        std::string port;
                        if (tmp.size() != 1)
                            port = trim(tmp.at(1), " ");

                        cout << "---> The request contains the Host header "
                                "with {host, port} == {"
                             << host << "," << port << "}" << '\n';

                        if (client.get_port() != port) {
                            // TODO USE SERVER NAME TO PICK THE WRITE SERVER TO
                            // RESPONDE
                            // PORT = CLIENT.PORT
                            // HOST = request.HOST
                            // map with HOST and PORT
                            port = client.get_port();
                            host = HostHeader;
                        }

                        serverInfo info;

                        {
                            map<pair<string, string>, serverInfo>::iterator it =
                                infos.find(std::make_pair(port, host));
                            if (it == infos.end()) {
                                info = infos.at(make_pair(port, ""));
                            } else {
                                info = it->second;
                            }
                        }

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

                        std::string const& loc = request.getLocation();
                        const map<string, location>& locations = info.locations;
                        const map<string, location>::const_iterator it =
                            locations.find(loc);
                        location route;
                        if (it == locations.end()) {
                            // TODO handle is a location doesnt exists in
                            // Locations
                            cerr << "[TODO] location doesnt exists\n";
                            assert(false);
                        } else {
                            route = it->second;
                        }
                        const std::string& method = request.getMethod();
                        std::string response;
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
                                    response =
                                        HttpResponse(200, "1.1", "OK").build();
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

                // HttpResponse(int status,
                //              std::string version,
                //              std::string action);

                // HttpResponse &add_header(std::string key, std::string value);

                // HttpResponse &add_to_body(std::string line);

                // HttpResponse &add_to_body(std::vector <std::string> body);
                /////////////////////////////////////////
                // std::string response =
                //     HttpResponse(200, "1.1", "OK")
                //         .add_header("Content-Type", "text/html")
                //         .add_to_body("<h1>hello, world</1>")
                //         .build();
                // cout << "[INFO] sending the response\n";
                // client.write(response.c_str(), response.size());
            }
            events.pop_back();
        }
    }
    ////////////////////////
}
