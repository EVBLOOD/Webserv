/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:04:48 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/10 23:57:57 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "tokengen.hpp"
#include <iterator>
#include <list>
#include "server.hpp"

#define A "server" // if no server no run
#define B "location" // in no location no place to go
#define C "listen" // if no listen no feedback
#define D "root" // if no root I'll be checking "/var/www/html"
#define E "client_max_body_size" // if no size one is the size
#define F "error_page" // if no error default I'll use mine
#define G "autoindex" // if no autoindex then yes
#define H "index" // you wanna travel dicide a place
#define I "allow_methods" // if not the everything
#define J "return" // you need to change path?
#define K "fastcgi_pass" // why speaking english if u only know arabic
#define L "upload_enable" // let's download not apload default == nope!
#define M "upload_store" // place to put your stuff in

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
    std::string         line;
    int                 i;
    int                 start;

    while (getline(config, line))
    {
        i = 0;
        // while (isspace(line[i]))
        //     i++;
        while (i >= 0 && line[i])
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
                while (line[i] && (line[i] != '"' || start == i))
                    i++;
                lexer.push_back(tokengen(QUOTES, line.substr(start, i - start + 1)));
            }
            else if (line[i] == '\'')
            {
                start = i;
                while (line[i] && (line[i] != '\'' || start == i))
                    i++;
                lexer.push_back(tokengen(QUOTES, line.substr(start, i - start + 1)));
            }
            else if (line[i] == '{')
                lexer.push_back(tokengen(OPENCURL));
            else if (line[i] == '}')
                lexer.push_back(tokengen(CLOSECURL));
            else if (line[i] == ';')
                lexer.push_back(tokengen(SEMICOLONS));
            else if (line[i] == ':')
                lexer.push_back(tokengen(COLON));
            else if (line[i] == '#')
            {
                lexer.push_back(tokengen(COMMENT));
                i = -1;
            }
            else
            {
                start = i;
                while (line[i] && isntspeacail(line[i]) == true)
                    i++;
                lexer.push_back(tokengen(WORD, line.substr(start, i - start)));
                continue;
            }
            if (i >= 0 && line[i])
                i++;
        }
        if (i < 0 || !line[i])
            lexer.push_back(tokengen(ENDOFLINE));
    }
    return lexer;
}
// enum Type {
//     OPENCURL,
//     CLOSECURL,
//     SEMICOLONS,
//     WHITESPACE,
//     WORD,
//     QUOTES,
//     COLON,
//     ENDOFLINE,
//     COMMENT
// };
std::vector<server> lexer_to_data(std::list<tokengen> lexer)
{
    std::vector<server> data;
    std::list<tokengen>::iterator begin = lexer.begin();
    std::list<tokengen>::iterator end = lexer.end();
    // server newone;
    while (begin != end && (begin->type == WHITESPACE || begin->type == COMMENT || begin->type == ENDOFLINE))
        begin++;
    while (begin != end)
    {
        if (begin->type == WORD && begin->content == A)
        {
            while (begin != end && (begin->type == WHITESPACE || begin->type == COMMENT || begin->type == ENDOFLINE))
                begin++;
            if (begin == end || begin->type != OPENCURL)
                exit (1); // you mad bro?
            begin++;
            while (begin != end && (begin->type == WHITESPACE || begin->type == COMMENT || begin->type == ENDOFLINE))
                begin++;
            if (begin == end)
                exit (0); // lets go
            // so far that's great;
            
        }
        if (begin != end)
            begin++;
    }
    if (data.size() == 0)
        exit (1); // throw an error bro no data here 
}

// std::list<tokengen> genarate_helper(std::list<tokengen> first)
// {
//     std::list<tokengen> last;
//     std::list<tokengen>::iterator begin = first.begin();
//     std::list<tokengen>::iterator end = first.end();
    
//     while (begin != end)
//     {
//         if 
//         begin++;
//     }
// }

parser::~parser()
{
    config.close();
}
