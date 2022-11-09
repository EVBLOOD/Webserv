/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:04:48 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 19:06:31 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"


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
bool isntspeacail(char x)
{
    return (x != '{' && x != '}' && x != ';' && x != ':' && x != '\'' && x != '"'  && isspace(x) == false);   
}
std::list<tokengen> parser::generate()
{
    std::list<tokengen> lexer;
    std::string line;
    int i;
    int start;
    int charscount;
    while (getline(config, line))
    {
        i = 0;
        while (line[i])
        {
            if (isspace(line[i]))
            {
                lexer.push_back(tokengen(WHITESPACE));
                while (isspace(line[i]))
                    i++;
            }
            if (line[i] == '"')
            {
                start = i;
                i++;
                while (line[i] && line[i] != '"')
                    i++;
                lexer.push_back(tokengen(QUOTES, line.substr(start, i + 1)));
            }
            else if (line[i] == '\'')
            {
                charscount = 1;
                start = i;
                while (line[i + charscount])
                {
                    charscount++;
                    if (line[i] == '\'')
                        break;
                }
                i += charscount;
                lexer.push_back(tokengen(QUOTES, line.substr(start, charscount)));
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
                while (line[i] && isntspeacail(line[i]))
                    i++;
                lexer.push_back(tokengen(WORD, line.substr(start, i + 1)));
                continue;
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
