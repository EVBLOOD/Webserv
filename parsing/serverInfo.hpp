

#pragma once

#include <list>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>
#include "location.hpp"
#include "tokengen.hpp"

class serverInfo {
   public:
    std::vector<std::string> index;
    std::string host;               // done
    std::vector<std::string> port;  // done
    std::vector<std::string> server_name;
    std::unordered_map<int, std::string> error_page;  // done
    std::string root;                                 // done
    std::map<std::string, Location> locations;        // done
    unsigned long long client_max_body_size;

   public:
    serverInfo(const serverInfo& sv);

    serverInfo();

    ~serverInfo();

    serverInfo& operator=(const serverInfo& info);

    template <int>
    void setters(std::list<tokengen>::iterator& big,
                 std::list<tokengen>::iterator& end);

    void setlocation(std::string x, Location y);

    void execute(int i,
                 std::list<tokengen>::iterator& big,
                 std::list<tokengen>::iterator& end);
};
