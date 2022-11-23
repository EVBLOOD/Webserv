//
// Created by Oussama Rahmouni on 11/23/22.
//

#include "HttpResponse.h"

HttpResponse::HttpResponse(int status,
                           std::string version,
                           std::string action) : _status(status), _version(version), _action(action), _headers(),
                                                 _body() {
};

HttpResponse &HttpResponse::add_header(std::string key, std::string value) {
    _headers[key] = value;
    return *this;
};

HttpResponse &HttpResponse::add_to_body(std::string line) {
    _body.push_back(line);
    return *this;
};

HttpResponse &HttpResponse::set_body(std::vector<std::string> body) {
    _body = body;
    return *this;
};

std::string HttpResponse::build() {
    std::string res;

    res += "HTTP/" + _version + " " + std::to_string(_status) + " " + _action + "\r\n";

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