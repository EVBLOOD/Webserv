#include <array>
#include <iostream>
#include <set>
#include "kqueue.hpp"
#include "tcpListener.hpp"

#define loop for (;;)

void handler(Kqueue& kq, void* data) {
    std::set<int> is_server;
    std::cout << "kdata " << kq.get_kdata() << '\n';
    std::vector<struct kevent> servers = kq.get_targets();

    for (size_t i = 0; i < servers.size(); ++i) {
        is_server.insert(servers[i].ident);
    }

    loop {
        std::pair<int, struct kevent*> _events = kq.get_events();
        int number_of_events = _events.first;
        struct kevent* events = _events.second;
        std::cout << "number of events " << number_of_events << '\n';

        if (number_of_events == -1) {
            std::cerr << " kevent before accept is joking!\n";
            exit(1);
        }

        for (int i = 0; i < number_of_events; i++) {
            if (is_server.find(events[i].ident) != is_server.end()) {
                const IServer& server =
                    (IServer&)kq.get_listener(events[i].ident);
                IStreamer& client = server.accept();
                if (client.get_raw_fd() == -1) {
                    std::cerr << "accept is joking!\n";
                    exit(1);
                }
                std::cout << "Server with fd == " << events[i].ident << " ";
                std::cout << "Accepted a connection with client with fd == "
                          << client.get_raw_fd() << '\n';
                kq.add_listener(&client);
                std::cout << "\nend\n";
            } else {
                TcpStream& client =
                    (TcpStream&)kq.get_listener(events[i].ident);
                std::cout << "[DEBUG] " << i << " " << events[i].ident << '\n';
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

                std::cout << "\nend;\n";
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
    Kqueue kq = Kqueue(listeners);
    kq.kqueue_job(handler, NULL);
}

// Example of a simple read from a client
//
// TcpListener listener = TcpListener("localhost", "8080");
// TcpStream tcp_stream = listener.accept();
// std::array<char, 4096> buff;
// memset(buff.data(), 0, buff.size());
// tcp_stream.read(buff.data(), buff.size());
// std::cout << "start ::\n";

// for (size_t i = 0; i < buff.size() && buff[i]; ++i) {
//     std::cout << buff[i];
// }

// std::cout << "\nend\n";
//
// void kJob(int kdata) {
//     struct kevent change, event;
//     struct kevent evSet;
//     loop {
//         int number_of_events = kevent(kdata, &change, 1, &event, 1, NULL);
//         std::cout << "number of events " << number_of_events << '\n';
//         if (number_of_events == -1) {
//             std::cerr << " kevent before accept is joking!\n";
//             exit(1);
//         }
//         std::cout << "[DEBUG] rwfd " << event.ident << '\n';
//         int rwfd = accept(event.ident, NULL, NULL);
//         if (rwfd == -1) {
//             std::cerr << "accept is joking!\n";
//             exit(1);
//         }
//         std::cout << "Server with fd == " << event.ident;
//         std::cout << "Accepted a connection with client with fd == " << rwfd
//                   << '\n';
//     }
// }

// for (size_t i = 0; i < events.size(); i++) {
//     is_server.insert(events[i].ident);
//     std::cout << "flags " << events[i].flags << " ";
//     std::cout << "ident " << events[i].ident << " ";
//     std::cout << "data " << events[i].data << " ";
//     std::cout << "fflags " << events[i].fflags << " ";
//     std::cout << "filter " << events[i].filter << " ";
//     std::cout << "udata " << events[i].udata;
//     std::cout << '\n';
// }
