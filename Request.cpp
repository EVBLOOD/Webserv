#include "Request.hpp"

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
    : _raw(),
      _method(),
      _location(),
      _version(),
      _headers(),
      _body(),
      _error(false) {
    assert(trim(" abc ", " ") == "abc");
    assert(trim("\nabc\n", "\n") == "abc");
    assert(trim("\rabc\r", "\r") == "abc");
    assert(trim("\rabc\n", "\r\n") == "abc");
    _raw = request;
    if (request[0] == '\0' || request.size() == 0) {
        _error = true;
        return;
    }
    std::vector<std::string> splited = split(request, "\n");
    std::vector<std::string> first_line = split(splited[0], " ");
    if (first_line.size() != 3) {
        _error = true;
        return;
    }
    _method = first_line[0];
    _location = first_line[1];
    _version = first_line[2];
    splited.erase(splited.begin());
    size_t i = 0;
    for (i = 0; i < splited.size() && splited[i] != "\r"; ++i) {
        std::vector<std::string> tmp = split(splited[i], ":");
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

    for (size_t j = i; j < splited.size(); ++j) {
        std::string trimed = trim(splited[j], "\n\r ");
        if (!trimed.empty())
            _body.push_back(trimed);
    }
}

void HttpRequest::dump() {
    std::cout << _method << " " << _location << " " << _version << "\n";
    for (std::map<std::string, std::string>::iterator iter = _headers.begin();
         iter != _headers.end(); ++iter) {
        std::cout << "[" << iter->first << "]"
                  << " : [" << iter->second << "]" << '\n';
    }
    for (std::vector<std::string>::iterator iter = _body.begin();
         iter != _body.end(); ++iter) {
        std::cout << *iter << "\n";
    }
}

bool HttpRequest::error() {
    return _error;
}

std::string HttpRequest::getRawData() {
    return _raw;
}

std::vector<std::string> HttpRequest::getBody() {
    return _body;
}

std::string HttpRequest::getHeaderValue(std::string key) {
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
