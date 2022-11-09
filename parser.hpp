/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 22:07:55 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 17:54:01 by sakllam          ###   ########.fr       */
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
class parser
{
    private:
        std::ifstream config;
        
    public:
        parser(const std::string &filename);
        std::list<tokengen> generate(std::ifstream config);
        ~parser();
};

parser::parser(const std::string &filename)
{
    config.open(filename); // open file
    if (config.is_open() == false ) // protection mode: file opened?
    { // no ? okey
        std::cout << "Some respect please ! \n";
        exit(1);
        // else throw an error;
    }
    if (config.peek() == EOF) // protection mode: nothing in the file
    {// EOF > okey
        std::cout << "Empty ---> \n";
        exit (1);
        // else throw an error;
    }
}

std::list<tokengen> parser::generate(std::ifstream config)
{
    std::list<tokengen> lexer;
    std::string line;
    int i;
    int start;
    while (getline(config, line))
    {
        i = 0;
        while (isspace(line[i]))
          i++;
        while (line[i])
        {
            if (isspace(line[i]))
            {
                lexer.push_back(tokengen(WHITESPACE));
                while (isspace(line[i]))
                    i++;
                continue;
            }
            if (line[i] == '"')
            {
                start = i;
                while (line[i] && line[i] != '"')
                    i++;
                lexer.push_back(tokengen(QUOTES, line.substr(start, i)));
            }
            else if (line[i] == '\'')
            {
                start = i;
                while (line[i] && line[i] != '\'')
                    i++;
                lexer.push_back(tokengen(QUOTES, line.substr(start, i)));
            }
            else if (line[i] == '{')
                lexer.push_back(tokengen(OPENCURL));
            else if (line[i] == '}')
                lexer.push_back(tokengen(CLOSECURL));
            else if (line[i] == ';')
                lexer.push_back(tokengen(SEMICOLONS));
            else if (line[i] == ':')
                lexer.push_back(tokengen(COLON));
            else
            {
                start = i;
                while (line[i] && line[i] != '{' && line[i] != '}'\
                    && line[i] != ';' && line[i] != ':' && line[i] != '\'' && line[i] != '"'  && !isspace(line[i]))
                    i++;
                lexer.push_back(tokengen(WORD, line.substr(start, i)));
            }
            if (line[i])
                i++;
        }
    }
    return lexer;
}

parser::~parser()
{
    config.close();
}
