//
// Created by Oussama Rahmouni on 11/23/22.
//

#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>

class HttpResponse {
private:
    int _status;
    std::string _version;
    std::string _action;
    std::map<std::string, std::string> _headers;
    std::vector<std::string> _body;
public:
    HttpResponse(int status,
                 std::string version,
                 std::string action);

    HttpResponse &add_header(std::string key, std::string value);

    HttpResponse &add_to_body(std::string line);

    HttpResponse &set_body(std::vector<std::string> body);

    std::string build();

    void dump();
};
