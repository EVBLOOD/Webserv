#pragma once

#include "socket_header.hpp"
#include "tools.h"

#include <vector>
#include <string>

using namespace tools;

// char resp[] =
//     "HTTP/1.0 200 OK\r\n"
//     "Server: webserver-c\r\n"
//     "Content-type: text/html\r\n\r\n"
//     "<h1>hello, world</1>\r\n"
//     "<ul><li>13</li>\r\n"
//     "<li>37</li></ul>\r\n";

//enum Method {
//    GET, POST, PUT, DELETE
//};

class HttpRequest {
public:
    std::string _raw;
    std::string _method;
    std::string _location;
    std::string _version;
    std::map<std::string, std::string> _headers;
    std::vector<std::string> _body;

public:
    explicit HttpRequest(std::string request);

    std::string getMethod();

    std::string getLocation();

    std::string getVersion();

    std::string getHeaderValue(std::string key);

    std::vector<std::string> getBody();

    std::string getRawData();

    void dump();
};
