#include "Response.hpp"
#include "Request.hpp"
#include "ServerBuilder.hpp"
#include "Server.hpp"
#include <assert.h>
#include <iostream>


char resp[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Length: 58\r\n"
        "Content-type: text/html\r\n"
        "Server: webserver-c\r\n"
        "\r\n"
        "<h1>hello, world</h1>\r\n"
        "<ul><li>13</li>\r\n"
        "<li>37</li></ul>\r\n";

char req[] =
        "POST /cgi-bin/process.cgi HTTP/1.1\r\n"
        "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
        "Host: www.google.com\r\n"
        "Content-Type: text/xml; charset=utf-8\r\n"
        "Content-Length: length\r\n"
        "Accept-Language: en-us\r\n"
        "Accept-Encoding: gzip, deflate\r\n"
        "Connection: Keep-Alive\r\n"
        "\r\n"
        "<?xml version=\"1.0\" encoding=\"utf-8\"?>\r\n"
        "<string xmlns=\"http://clearforest.com/\">string</string>\r\n";

int main() {
    {
        std::string body =
                "<h1>hello, world</h1>\r\n"
                "<ul><li>13</li>\r\n"
                "<li>37</li></ul>\r\n";
        HttpResponse test(200, "1.1", "OK");
        test.add_header("Server", "webserver-c")
                .add_header("Content-type", "text/html")
                .add_to_body("<h1>hello, world</h1>")
                .add_to_body("<ul><li>13</li>")
                .add_to_body("<li>37</li></ul>")
                .build();
        assert(test.build() == resp);
        assert(test.get_body_size() == body.length());
//        std::cout << "RESPONSE\n";
//        test.dump();
//        std::cout << "END RESPONSE\n";
    }
    {
        HttpRequest request(req);
//        std::cout << "REQUEST\n";
//        request.dump();
//        std::cout << "END REQUEST\n";
    }
    {
        Server test = ServerBuilder().set_backlog(100).set_host("127.0.0.1").set_port(8080).build();
        assert(test.getHost() == htonl(0x7f000001));
        assert(test.getPort() == htons(8080));
        assert(test.getBacklog() == 100);
    }
//    {
//        std::vector <std::string> index_html = open_to_serve("index.html");
//        for (size_t i = 0; i < index_html.size(); ++i) {
//            std::cout << index_html[i] << '\n';
//        }
//    }
    std::cout << "Response test 👍" << '\n';
    std::cout << "Request test 👍" << '\n';
    std::cout << "ServerBuilder test 👍" << '\n';
    std::cout << " --- parsing host test 👍" << '\n';
    std::cout << " --- parsing port test 👍" << '\n';
}