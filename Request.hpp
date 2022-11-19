#pragma once
#include "socket_header.hpp"

// char resp[] =
//     "HTTP/1.0 200 OK\r\n"
//     "Server: webserver-c\r\n"
//     "Content-type: text/html\r\n\r\n"
//     "<h1>hello, world</1>\r\n"
//     "<ul><li>13</li>\r\n"
//     "<li>37</li></ul>\r\n";

enum Method { GET, POST, PUT, DELETE };

struct Body {
    std::string content_type;
    std::vector<std::string> content;
    Body(std::string content_type, std::vector<std::string> content)
        : content_type(content_type), content(content){};
    Body() : content_type(), content(){};
};

class HttpRequest {
   public:
    std::string raw;
    std::string method;
    std::string location;
    std::string version;
    Body body;
    std::map<std::string, std::string> headers;

   public:
    static std::vector<std::string> split(std::string str, std::string del);
    static std::string trim(std::string str, std::string del);
    HttpRequest(std::string request);
    void dump();
};
