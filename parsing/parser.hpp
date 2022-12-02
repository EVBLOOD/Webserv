/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 22:07:55 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/13 22:45:04 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
