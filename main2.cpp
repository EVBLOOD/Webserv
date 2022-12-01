#include <set>
#include "New/TcpStream.hpp"
#include "New/kqueue.hpp"
#include "New/tcpListener.hpp"
#include "Server.hpp"
#include "ServerBuilder.hpp"
#include "ServerPoll.hpp"
#include "parsing/parser.hpp"
#include "parsing/tokengen.hpp"

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
        return HttpResponse(301, "1.1", "Moved Permanently")
            .add_header("Location", "/oussama");
    }

    return HttpResponse(404, "1.1", "Not Found")
        //            .add_header("Location", "/404")
        .add_header("Content-Type", "text/html")
        .add_to_body("<h1>404</1>");
}

HttpResponse test_response_server_2(HttpRequest request) {
    std::string location = request.getLocation();

    if (location == "/") {
        return HttpResponse(200, "1.1", "OK")
            .add_header("Content-Type", "text/html")
            .add_to_body("<h1>This is server 2</1>");
    }

    return HttpResponse(404, "1.1", "Not Found")
        .add_header("Content-Type", "text/html")
        .add_to_body("<h1>404</1>");
}

typedef std::map<std::pair<std::string, std::string>,
                 std::function<HttpResponse(HttpRequest)> >
    idea_t;

void handler(Kqueue& kq, const TcpListener& server, idea_t idea);
void handler(Kqueue& kq, TcpStream& client, idea_t idea);
void handler(Kqueue& kq, void* data) {
    idea_t idea = *(idea_t*)data;
    loop {
        std::vector<IListener*> _events = kq.get_events();
        int number_of_events = _events.size();
        std::cout << "number of events " << number_of_events << '\n';

        for (int i = 0; i < number_of_events; i++) {
            if (dynamic_cast<IServer*>(_events[i]))
                handler(kq, *dynamic_cast<TcpListener*>(_events[i]), idea);
            else {
                handler(kq, *dynamic_cast<TcpStream*>(_events[i]), idea);
            }
        }
    }
}

int main() {
    parser file("conf");
    std::list<tokengen> tokens = file.generate();
    std::vector<serverInfo> servers_info = file.lexer_to_data(tokens);
    idea_t idea;
    int i = 0;
    std::array<std::function<HttpResponse(HttpRequest)>, 2> functions = {
        test_response_server_1, test_response_server_2};
    for (auto ser : servers_info) {
        for (auto p : ser.port) {
            idea.insert(
                std::make_pair(std::make_pair(p, "0.0.0.0"), functions[i]));
        }
        for (auto p : ser.port) {
            idea.insert(std::make_pair(std::make_pair(p, ""), functions[i]));
            for (auto n : ser.server_name) {
                idea.insert(std::make_pair(std::make_pair(p, n), functions[i]));
            }
        }
        ++i;
    }

    // for (auto x : idea) {
    //     std::cout << " port " << x.first.first << " server name "
    //               << x.first.second << '\n';
    // }
    TcpListener server1 = TcpListener("localhost", "8080");
    TcpListener server2 = TcpListener("localhost", "8081");
    std::vector<IListener*> listeners;
    listeners.push_back(&server1);
    listeners.push_back(&server2);
    Kqueue kq = Kqueue();
    kq.attach(&server1);
    kq.attach(&server2);
    handler(kq, (void*)&idea);
}

void handler(Kqueue& kq, const TcpListener& server, idea_t idea) {
    // TODO: we should add the the port from server to user!
    IStreamer& client = server.accept();
    if (client.get_raw_fd() == -1) {
        std::cerr << "accept is joking!\n";
        exit(1);
    }
    std::cout << "Accepted a connection with client with fd == "
              << client.get_raw_fd();
    handler(kq, *(TcpStream*)&client, idea);
}

void handler(Kqueue& kq, TcpStream& client, idea_t idea) {
    std::array<char, 4096> buff;
    memset(buff.data(), 0, buff.size());

    if (client.read(buff.data(), buff.size()) <= 0) {
        kq.detach(&client);
        client.shutdown();
        return;
    }

    std::cout << "------------------------------------------------------\n";
    for (size_t b = 0; b < buff.size() && buff[b]; ++b) {
        std::cout << buff[b];
    }
    std::cout << "------------------------------------------------------\n";

    HttpRequest req(buff.data());
    std::cout << "client HOST value == [" << req.getHeaderValue("Host")
              << "]\n";
    std::string host;
    std::string port;
    if (req.getHeaderValue("Host").empty() == false) {
        std::vector<std::string> tmp = split(req.getHeaderValue("Host"), ":");
        if (tmp.size() == 1) {
            host = tmp[0];
            port = client.get_port();
        } else {
            host = tmp[0];
            port = tmp[1];
        }
    } else {
        port = client.get_port();
        host = client.get_host();
    }
    if (idea.find(std::make_pair(port, host)) == idea.end()) {
        exit(1);
    }
    HttpResponse res = idea[std::make_pair(port, host)](req);
    std::cout << "\nend\n";
    client.write(res.build().c_str(), res.build().size());
    std::cout << "Connection status : [" << req.getHeaderValue("Connection")
              << "]\n";
    if (req.getHeaderValue("Connection") == "keep-alive") {
        kq.attach(&client);
    } else {
        std::cout << "client with fd == " << client.get_raw_fd()
                  << " was shutdown\n";
        kq.detach(&client);
        client.shutdown();
    }
}
