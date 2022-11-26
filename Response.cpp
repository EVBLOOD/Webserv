//
// Created by Oussama Rahmouni on 11/23/22.
//

#include "Response.hpp"

HttpResponse::HttpResponse(int status,
                           std::string version,
                           std::string action) : _status(status), _content_length(0), _version(version),
                                                 _action(action), _headers(),
                                                 _body() {
};

HttpResponse &HttpResponse::add_header(std::string key, std::string value) {
    _headers[key] = value;
    return *this;
};

HttpResponse &HttpResponse::add_to_body(std::string line) {
    _body.push_back(line);
    _content_length += line.length() + 2;
    return *this;
};

HttpResponse &HttpResponse::add_to_body(std::vector <std::string> body) {
    std::vector<std::string>::iterator iter = body.begin();
    while (iter != body.end()) {
        add_to_body(*iter);
        ++iter;
    }
    return *this;
};

size_t HttpResponse::get_body_size() {
    return _content_length;
};

std::string HttpResponse::build() {
    std::string res;

    res += "HTTP/" + _version + " " + std::to_string(_status) + " " + _action + "\r\n";

    _headers["Content-Length"] = std::to_string(_content_length);
    {
        std::map<std::string, std::string>::iterator iter = _headers.begin();
        while (iter != _headers.end()) {
            res += iter->first + ": " + iter->second + "\r\n";
            iter++;
        }

    }
    res += "\r\n";
    {
        std::vector<std::string>::iterator iter = _body.begin();
        while (iter != _body.end()) {
            res += *iter + "\r\n";
            iter++;
        }
    }
    return res;
}

void HttpResponse::dump() {
    std::cout << "HTTP/" + _version + " " + std::to_string(_status) + " " + _action + "\n";
    for (std::map<std::string, std::string>::iterator iter = _headers.begin();
         iter != _headers.end(); ++iter) {
        std::cout << "[" << iter->first << "]" << " : [" << iter->second << "]" << '\n';
    }
    for (std::vector<std::string>::iterator iter = _body.begin();
         iter != _body.end(); ++iter) {
        std::cout << *iter << "\n";
    }
}
//  int _status;
//    std::string _version;
//    std::string _action;
//    std::map<std::string, std::string> _headers;
//    std::vector<std::string> _body;