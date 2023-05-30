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
typedef multimap::const_iterator const_multi_iter;
// typedef multimap::iterator multi_iter;

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
    explicit HttpRequest(const std::string& request);

    const std::string& getMethod() const;

    const std::string& getLocation() const;

    const std::string& getVersion() const;

    std::string getHeaderValue(const std::string& key) const;

    std::pair<const_multi_iter, const_multi_iter> getHeaderValues(
        const std::string& key) const;

    const std::string& getBody() const;

    const std::string& getRawData() const;

    bool error() const;

    void dump() const;
};
