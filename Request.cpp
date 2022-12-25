#include "Request.hpp"
#include <sys/_types/_size_t.h>
#include <cassert>
#include <exception>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include "tools.hpp"

// char resp[] =
//         "HTTP/1.0 200 OK\r\n"
//         "Server: webserver-c\r\n"
//         "Content-type: text/html\r\n\r\n"
//         "<h1>hello, world</1>\r\n"
//         "<ul><li>13</li>\r\n"
//         "<li>37</li></ul>\r\n";

// std::string _raw;
// std::string _method;
// std::string _location;
// std::string _version;
// std::map<std::string, std::string> _headers;
// std::vector<std::string> _body;

HttpRequest::HttpRequest(std::string request)
    : _raw(request),
      _method(),
      _location(),
      _version(),
      _headers(),
      _body(),
      _error(false) {
    if (request.empty() || request[0] == '\0') {
        _error = true;
        return;
    }
    // std::cout << "-------------------------------\n";
    // std::cout << request << "\n";
    // std::cout << "-------------------------------\n";
    std::vector<std::string> header_and_body =
        tools::split_(request, "\r\n\r\n");

    if (header_and_body.size() == 0) {
        _error = true;
        return;
    }

    std::vector<std::string> splited = split(header_and_body[0], "\n");

    if (splited.size() == 0) {
        _error = true;
        return;
    }

    std::vector<std::string> first_line = split(splited[0], " ");

    if (first_line.size() != 3) {
        _error = true;
        return;
    }

    _method = first_line[0];
    _location = first_line[1];
    _version = first_line[2];

    // if (_version != "HTTP/1.1") {
    //     _error = true;
    //     return;
    // }

    for (size_t i = 1; i < splited.size() && splited[i] != "\r"; ++i) {
        std::vector<std::string> tmp = split(splited[i], ":");
        // TODO: make key lower case
        std::string key = trim(tmp[0], "\n\r ");
        std::string value;
        if (tmp.size() > 2) {
            for (size_t j = 1; j < tmp.size(); ++j)
                value += tmp[j] + ":";
            value = trim(value, "\n\r :");
        } else {
            value = trim(tmp[1], "\n\r ");
        }

        _headers[key] = value;
    }

    if (getHeaderValue("Host").empty() ||
        getHeaderValue("User-Agent").empty()) {
        _error = true;
        return;
    }

    if (getMethod() == "POST") {
        if (getHeaderValue("Content-Type").empty() ||
            getHeaderValue("Content-Length").empty()) {
            _error = true;
            return;
        }
        try {
            std::stoull(getHeaderValue("Content-Length"));
        } catch (std::exception& e) {
            _error = true;
            return;
        }
    }

    for (size_t i = 1; i < header_and_body.size(); i++)
        _body += header_and_body[i];
}

void HttpRequest::dump() {
    std::cout << _method << " " << _location << " " << _version << "\n";
    for (std::map<std::string, std::string>::iterator iter = _headers.begin();
         iter != _headers.end(); ++iter) {
        std::cout << "[" << iter->first << "]"
                  << " : [" << iter->second << "]" << '\n';
    }
    // for (std::vector<std::string>::iterator iter = _body.begin();
    //      iter != _body.end(); ++iter) {
    //     std::cout << *iter << "\n";
    // }
}

bool HttpRequest::error() {
    return _error;
}

std::string HttpRequest::getRawData() {
    return _raw;
}

std::string HttpRequest::getBody() {
    return _body;
}

std::string HttpRequest::getHeaderValue(std::string key) {
    // TODO: make key lowercase
    // struct cmp {}
    return _headers[key];
}

std::string HttpRequest::getVersion() {
    return _version;
}

std::string HttpRequest::getLocation() {
    return _location;
}

std::string HttpRequest::getMethod() {
    return _method;
}

// int main() {
//     HttpRequest(
//         "HTTP/1.0 200 OK\r\n"
//         "Server: webserver-c\r\n"
//         "Content-type: text/html\r\n\r\n"
//         "<h1>hello, world</1>\r\n"
//         "<ul><li>13</li>\r\n"
//         "<li>37</li></ul>\r\n")
//         .dump();
// }
