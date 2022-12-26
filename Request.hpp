#pragma once

#include <sys/_types/_size_t.h>
#include "socket_header.hpp"
#include "tools.hpp"

#include <functional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

using namespace tools;
typedef std::unordered_multimap<std::string, std::string> multimap;
typedef multimap::iterator multi_iter;

class HttpRequest {
   private:
    std::string _raw;
    std::string _method;
    std::string _location;
    std::string _version;
    multimap _headers;
    std::string _body;
    bool _error;

   public:
    explicit HttpRequest(std::string request);

    std::string getMethod();

    std::string getLocation();

    std::string getVersion();

    std::string getHeaderValue(std::string key);

    std::pair<multi_iter, multi_iter> getHeaderValues(std::string key);

    std::string getBody();

    std::string getRawData();

    bool error();

    void dump();
};
