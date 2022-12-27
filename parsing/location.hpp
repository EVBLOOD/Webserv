

#pragma once

#include <cctype>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "tokengen.hpp"

enum func {
    setport,
    setservername,
    setmap,
    setroot,
    setclient_max_body_size,
    sethost,
    setallow_methods,
    setfastcgi_pass,
    setindex,
    setreturn,
    setautoindex,
    setupload_enable,
    setupload_store
};

class Location {
   public:
    std::vector<std::string> index;
    std::map<int, std::string> ret_rn;
    bool autoindex;
    std::vector<std::string> allow_methods;
    std::string fastcgi_pass;
    bool upload_enable;
    std::string upload_store;

   public:
    Location(const Location& lc);

    Location();

    ~Location();

    Location& operator=(const Location& lc);

    template <int>
    void set(std::list<tokengen>::iterator& big,
             std::list<tokengen>::iterator& end);  // turn this to refrence
    void execute(int i,
                 std::list<tokengen>::iterator& big,
                 std::list<tokengen>::iterator& end);
};

void CURLWAIT(std::list<tokengen>::iterator& x,
              std::list<tokengen>::iterator& end,
              bool loc = false);
