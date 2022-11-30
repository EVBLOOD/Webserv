#include <set>
#include "New/TcpStream.hpp"
#include "New/kqueue.hpp"
#include "New/tcpListener.hpp"
#include "Server.hpp"
#include "ServerBuilder.hpp"
#include "ServerPoll.hpp"

#define loop for (;;)

HttpResponse test_response_server_1(HttpRequest request) {
    std::string location = request.getLocation();

    if (location == "/") {
        return HttpResponse(200, "1.1", "OK")
            //                .add_header("Location: ", "/")
            .add_header("Content-Type", "text/html")
            .add_to_body("<h1>hello, world</1>");
    }
    if (location == "/index.html") {
        return HttpResponse(200, "1.1", "OK")
            //                .add_header("Location: ", "/index.html")
            .add_header("Content-Type", "text/html")
            .add_to_body(tools::open_to_serve("index.html"));
    }
    if (location == "/app.css") {
        return HttpResponse(200, "1.1", "OK")
            //                .add_header("Location: ", "/index.html")
            .add_header("Content-Type", "text/css")
            .add_to_body(tools::open_to_serve("app.css"));
    }
    if (location == "/app.js") {
        return HttpResponse(200, "1.1", "OK")
            //                .add_header("Location: ", "/index.htm")
            .add_header("Content-Type", "application/javascript")
            .add_to_body(tools::open_to_serve("app.js"));
    }
    if (location == "/oussama" && request.getMethod() == "GET") {
        return HttpResponse(200, "1.1", "OK")
            //                .add_header("Location: ", "/oussama")
            .add_header("Content-Type", "text/html")
            .add_to_body(" <form  method = \"POST\">")
            .add_to_body("Name: <input type = \"text\" name = \"name\" />")
            .add_to_body("Weight: <input type = \"text\" name = \"weight\" />")
            .add_to_body("<input type = \"submit\" />")
            .add_to_body("</form>");
    }
    if (location == "/oussama" && request.getMethod() == "POST") {
        std::string content = request.getBody()[0];
        std::vector<std::string> name_weight = split(content, "&");
        return HttpResponse(200, "1.1", "OK")
            //                .add_header("Location: ", "/oussama")
            .add_header("Content-Type", "text/html")
            .add_to_body("<h1>hello " + split(name_weight[0], "=")[1] + "</h1>")
            .add_to_body("<h1>your weight is " + split(name_weight[1], "=")[1] +
                         "</h1>");
    }
    if (location == "/saad") {
        return HttpResponse(301, "1.1", "Moved Permanently");
        //                add_header("Location", "/oussama");
    }

    return HttpResponse(404, "1.1", "Not Found")
        //            .add_header("Location", "/404")
        .add_header("Content-Type", "text/html")
        .add_to_body("<h1>404</1>");
}

// void handler(Kqueue& kq, void* data) {
//     std::set<int> is_server;
//     std::cout << "kdata " << kq.get_kdata() << '\n';
//     std::vector<struct kevent> servers = kq.get_targets();

//     for (size_t i = 0; i < servers.size(); ++i) {
//         is_server.insert(servers[i].ident);
//     }

//     loop {
//         std::pair<int, struct kevent*> _events = kq.get_events();
//         int number_of_events = _events.first;
//         struct kevent* events = _events.second;
//         std::cout << "number of events " << number_of_events << '\n';

//         if (number_of_events == -1) {
//             std::cerr << " kevent before accept is joking!\n";
//             exit(1);
//         }

//         for (int i = 0; i < number_of_events; i++) {
//             if (is_server.find(events[i].ident) != is_server.end()) {
//                 const IServer& server =
//                     (IServer&)kq.get_listener(events[i].ident);
//                 IStreamer& client = server.accept();
//                 if (client.get_raw_fd() == -1) {
//                     std::cerr << "accept is joking!\n";
//                     exit(1);
//                 }
//                 std::cout << "Server with fd == " << events[i].ident << " ";
//                 std::cout << "Accepted a connection with client with fd == "
//                           << client.get_raw_fd() << '\n';
//                 kq.add_listener(&client);
//                 std::cout << "\nend\n";
//             } else {
//                 TcpStream& client =
//                     (TcpStream&)kq.get_listener(events[i].ident);
//                 std::cout << "[DEBUG] " << i << " " << events[i].ident <<
//                 '\n'; std::array<char, 4096> buff; memset(buff.data(), 0,
//                 buff.size());

//                 if (client.read(buff.data(), buff.size()) <= 0) {
//                     client.shutdown();
//                     continue;
//                 }

//                 std::cout << "start :\n";

//                 for (size_t b = 0; b < buff.size() && buff[b]; ++b) {
//                     std::cout << buff[b];
//                 }

//                 HttpRequest req(std::string(buff.data()));
//                 HttpResponse res = test_response_server_1(req);
//                 std::cout << "\nend;\n";
//                 client.write(res.build().c_str(), res.build().size());
//             }
//         }
//     }
// }
//
//
void handler(Kqueue& kq, void* data) {
    std::set<int> is_server;
    std::cout << "kdata " << kq.get_kdata() << '\n';
    std::vector<struct kevent> servers = kq.get_targets();

    for (size_t i = 0; i < servers.size(); ++i) {
        is_server.insert(servers[i].ident);
    }

    loop {
        std::vector<IListener*> _events = kq.get_listeners();
        int number_of_events = _events.size();
        std::cout << "number of events " << number_of_events << '\n';

        if (number_of_events == -1) {
            std::cerr << " kevent before accept is joking!\n";
            exit(1);
        }

        for (int i = 0; i < number_of_events; i++) {
            if (dynamic_cast<IServer*>(_events[i])) {
                const IServer& server = *(IServer*)_events[i];
                IStreamer& client = server.accept();
                if (client.get_raw_fd() == -1) {
                    std::cerr << "accept is joking!\n";
                    exit(1);
                }
                std::cout << "Accepted a connection with client with fd == "
                          << client.get_raw_fd() << '\n';
                kq.add_listener(&client);
                std::cout << "\nend\n";
            } else {
                TcpStream& client = *(TcpStream*)_events[i];
                std::array<char, 4096> buff;
                memset(buff.data(), 0, buff.size());

                if (client.read(buff.data(), buff.size()) <= 0) {
                    client.shutdown();
                    continue;
                }

                std::cout << "start :\n";

                for (size_t b = 0; b < buff.size() && buff[b]; ++b) {
                    std::cout << buff[b];
                }

                HttpRequest req(std::string(buff.data()));
                HttpResponse res = test_response_server_1(req);
                std::cout << "\nend;\n";
                client.write(res.build().c_str(), res.build().size());
            }
        }
    }
}

int main() {
    TcpListener server1("localhost", "8080");
    TcpListener server2("localhost", "8081");
    std::vector<IListener*> listeners;

    listeners.push_back(&server1);
    listeners.push_back(&server2);
    Kqueue(listeners).kqueue_job(handler, NULL);
}
