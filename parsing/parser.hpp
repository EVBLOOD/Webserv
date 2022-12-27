

#pragma once

#include <cctype>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include "location.hpp"
#include "serverInfo.hpp"
#include "tokengen.hpp"

class parser {
   private:
    std::ifstream config;
    std::vector<serverInfo> servers;
    Location tmploc;
    serverInfo tmpserv;

   public:
    parser(const std::string& filename);

    std::list<tokengen> generate();

    ~parser();

    template <int>
    void separating(std::list<tokengen>::iterator& begin,
                    std::list<tokengen>::iterator& end,
                    bool server = true);

    std::vector<serverInfo> lexer_to_data(std::list<tokengen> lexer);
};
