//
// Created by Oussama Rahmouni on 11/23/22.
//

#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class HttpResponse {
   private:
    int _status;
    size_t _content_length;
    std::string _version;
    std::string _action;
    std::map<std::string, std::string> _headers;
    std::vector<std::string> _body;

   public:
    HttpResponse(int status, std::string version, std::string action);

    HttpResponse& add_header(std::string key, std::string value);

    HttpResponse& add_to_body(std::string line);

    HttpResponse& add_to_body(std::vector<std::string> body);

    HttpResponse& add_content_type(std::string path);

    size_t get_body_size();

    std::string build();

    void dump();

   public:
    static std::string get_content_type(std::string location);
    static HttpResponse error_response(int status, std::string file);
    static HttpResponse send_file(std::string file,
                                  std::string root,
                                  std::map<int, std::string> error_pages);

    static HttpResponse redirect_moved_response(std::string const& location);
    static HttpResponse redirect_found_response(std::string const& location);
};
