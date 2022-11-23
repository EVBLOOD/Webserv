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

#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include "tokengen.hpp"
#include <cctype>
#include "server.hpp"
#include "location.hpp"
class parser
{
    private:
        std::ifstream config;
        std::vector<server> servers;
        location tmploc;
        server tmpserv;
    public:
        parser(const std::string &filename);
        std::list<tokengen> generate();
        ~parser();
        template <int>
        void separating(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end, bool server = true);
        std::vector<server> lexer_to_data(std::list<tokengen> lexer);
};

