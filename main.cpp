#include <iostream>
#include <set>
#include "New/TcpStream.hpp"
#include "New/kqueue.hpp"
#include "New/streamer_interface.hpp"
#include "New/tcpListener.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "parsing/location.hpp"
#include "parsing/parser.hpp"
#include "parsing/serverInfo.hpp"
#include "parsing/tokengen.hpp"
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

void handle_new_connection(Kqueue &kq, TcpListener *server, map<pair<string, string>, serverInfo> infos);
void handle_requests(Kqueue &event_queue, IStreamer &client, map<pair<string, string>, serverInfo> infos);

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
                handle_new_connection(event_queue,  dynamic_cast<TcpListener*>(event), infos);
            } else 
            {
                handle_requests(event_queue, *dynamic_cast<IStreamer*>(event), infos);
            }
            events.pop_back();
        }
    }
    ////////////////////////
}

void handle_new_connection(Kqueue &kq, TcpListener *server, map<pair<string, string>, serverInfo> infos)
{
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
    kq.attach(&client);
}

void handle_requests(Kqueue &event_queue, IStreamer &event, map<pair<string, string>, serverInfo> infos)
{
    TcpStream& client = dynamic_cast<TcpStream&>(event);
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
            }
            else
            {
                ////// if Route is in Locations
                Location route = it->second;
                const std::string& method = request.getMethod();
                // if (find(route.allow_methods.begin(),
                //          route.allow_methods.end(),
                //          method) == route.allow_methods.end()) {
                //     // TODO handle a no allowed method
                //     cerr << "[TODO] method is not allowed\n";
                //     assert(false);
                // } else {
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
                    if (route.index.size() >= 1) {
                        cerr << "[DEBUG] handle index\n";
                        response = HttpResponse::index_response(route.index, info.root, info.error_page).build();
                    } else if (route.index.empty() && route.ret_rn.size() == 1) {
                        assert(route.ret_rn.size() == 1);
                        pair<int, string> ret = *route.ret_rn.begin();
                        cout << "[DEBUG] redirect " << ret.first << " "
                            << ret.second << '\n';

                        response =
                            handle_redirection(ret.first, ret.second)
                                .build();
                    }
                    else 
                    {
                        cerr << "[ERROR] no index + no return \n";
                        exit (1);
                    }
                }
            }
            // }
            client.write(response.data(), response.size());
        }
    }
}