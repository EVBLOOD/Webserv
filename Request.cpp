#include "Request.hpp"
#include <sys/_types/_size_t.h>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "parsing/tokengen.hpp"
#include "tools.hpp"

HttpRequest::HttpRequest(const std::string& request)
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
    std::vector<std::string> header_and_body =
        tools::split_(request, "\r\n\r\n");

    if (header_and_body.size() == 0) {
        _error = true;
        return;
    }

    std::string& headers = header_and_body[0];
    std::vector<std::string> splited = split(headers, "\r\n");

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

    for (size_t i = 1; i < splited.size(); ++i) {
        std::vector<std::string> tmp = split(splited[i], ":");
        std::string key = toUppercase(trim(tmp[0], "\n\r "));
        std::string value;
        if (tmp.size() > 2) {
            for (size_t j = 1; j < tmp.size(); ++j)
                value += tmp[j] + ":";
            value = trim(value, "\n\r :");
        } else {
            value = trim(tmp[1], "\n\r ");
        }

        _headers.insert(make_pair(key, value));
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

void HttpRequest::dump() const {
    std::cout << _method << " " << _location << " " << _version << "\n";
    for (const_multi_iter iter = _headers.begin(); iter != _headers.end();
         iter++) {
        std::cout << "[" << iter->first << "]"
                  << " : [" << iter->second << "]" << '\n';
    }
}

bool HttpRequest::error() const {
    return _error;
}

const std::string& HttpRequest::getRawData() const {
    return _raw;
}

const std::string& HttpRequest::getBody() const {
    return _body;
}

std::string HttpRequest::getHeaderValue(const std::string& key) const {
    const_multi_iter iter = _headers.find(toUppercase(key));
    if (iter == _headers.end())
        return "";
    return std::string(iter->second);
}

std::pair<const_multi_iter, const_multi_iter> HttpRequest::getHeaderValues(
    const std::string& key) const {
    return _headers.equal_range(tools::toUppercase(key));
}

const std::string& HttpRequest::getVersion() const {
    return _version;
}

const std::string& HttpRequest::getLocation() const {
    return _location;
}

const std::string& HttpRequest::getMethod() const {
    return _method;
}
