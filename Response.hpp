
#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "Request.hpp"
#include "socket/kqueue.hpp"

class HttpResponse {
   public:
    static std::unordered_map<std::string, std::string> files_cache;

   private:
    int _status;
    size_t _content_length;
    std::string _version;
    std::string _action;
    std::unordered_multimap<std::string, std::string> _headers;
    std::vector<std::string> _body;

   public:
    HttpResponse(int status,
                 const std::string& version,
                 const std::string& action);

    HttpResponse(int status,
                 const std::string& version,
                 const std::string& action,
                 const HttpRequest& request);

    HttpResponse& add_header(const std::string& key, const std::string& value);

    std::string getHeaderValue(const std::string& key) const;

    HttpResponse& add_to_body(const std::string& line);

    HttpResponse& add_to_body(const std::vector<std::string>& body);

    HttpResponse& add_content_type(const std::string& path);

    size_t get_body_size() const;

    std::string build();

    void dump() const;

   public:
    static std::string generateErrorPage(int statusCode,
                                         const std::string& statusMessage);
    static std::string get_content_type(const std::string& location);
    static HttpResponse error_response(int status, const std::string& file);
    static HttpResponse send_file(
        Kqueue& q,
        const std::string& file,
        const std::string& root,
        std::unordered_map<int, std::string>& error_pages);

    static HttpResponse redirect_moved_response(const std::string& location);
    static HttpResponse redirect_found_response(const std::string& location);
    static HttpResponse index_response(
        Kqueue& q,
        const std::vector<std::string>& index,
        const std::string& root,
        std::unordered_map<int, std::string>& error_pages);
    static HttpResponse generate_indexing(const std::string& dir,
                                          const std::string& location);
    static void updateFileCache(const std::string& full_path);
};
