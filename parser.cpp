/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:04:48 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/14 23:25:06 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "location.hpp"
#include "tokengen.hpp"
#include <iterator>
#include <list>
// #include <sys/_types/_size_t.h>
#include <vector>
#include "server.hpp"


enum KEYEGN{
  context_server,
  context_location,
  simpledir
};
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
#define N "server_name" // place to put your stuff in

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
    return (x != '{' && x != '}' && x != ';' && x != '\'' && x != '"'  && isspace(x) == false);   
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
        while (i >= 0 && line[i])
        {
            if (std::isspace(line[i]))
            {
                lexer.push_back(tokengen(WHITESPACE));
                while (std::isspace(line[i]))
                    i++;
                continue;
            }
            else if (line[i] == '"')
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

// void CURLWAIT(std::list<tokengen>::iterator &x, std::list<tokengen>::iterator &end, bool loc = false)
// {
//     while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
//         x++;
//     if (loc)
//         return ;
//     if (x == end ||x->type != OPENCURL)
//         exit (1); // you mad bro?
//     x++;
//     while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
//         x++;
//     if (x == end)
//         exit (0); // lets go
// }

std::vector<std::pair<bool, std::string> > generatestring(bool server)
{
    std::vector<std::pair<bool, std::string> > x;
    x.reserve(13);
    x.push_back(std::make_pair(true, C));
    x.push_back(std::make_pair(true, N));
    x.push_back(std::make_pair(true, F));
    x.push_back(std::make_pair(false, D));
    x.push_back(std::make_pair(false, E));
    x.push_back(std::make_pair(false, "location"));
    if (server)
        return x;
    x.push_back(std::make_pair(true, I));
    x.push_back(std::make_pair(false, K));
    x.push_back(std::make_pair(false, H));
    x.push_back(std::make_pair(false, J));
    x.push_back(std::make_pair(false, G));
    x.push_back(std::make_pair(false, L));
    x.push_back(std::make_pair(false, M));
    return (x);
}


template <>
void parser::separating<simpledir>(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end, bool server) // I may need somewhere to store this data!
{
    std::string tmp;
    int i;

    CURLWAIT(begin, end, true);
    if (begin == end || (begin->type != WORD && begin->type != QUOTES))
        exit (1);
    std::vector<std::pair<bool, std::string> > cond =  generatestring(server);
    size_t size = cond.size(); // I can optimize this one
    if (begin->type == QUOTES)
        tmp = begin->content.substr(1, begin->content.length() - 2);
    else
        tmp = begin->content;
    for (i = 0; i < size; i++)
    {
        if (cond[i].second == tmp)
        {
            if (server)
                tmpserv.execute(i, begin, end);
            else
                tmploc.execute(i, begin, end);
            break ;
        }
    }
    if (begin == end || i == size)
        exit (1);
    // begin++;
    CURLWAIT(begin, end, true);
}
template <>
void parser::separating<context_location>(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end, bool server)
{
    std::string name;
    tmploc = location();
    CURLWAIT(begin, end, true);
    if (begin == end || (begin->type != WORD && begin->type != QUOTES))
        exit (1);
    if (begin->type == QUOTES)
        name = begin->content.substr(1, begin->content.length() - 2);
    else
        name = begin->content;
    begin++;
    CURLWAIT(begin, end);
    while (begin != end && begin->type != CLOSECURL)
        separating<simpledir>(begin, end, false);
    if (begin == end)
        exit(0); // not closed
    begin++;
    CURLWAIT(begin, end, true);
    tmpserv.setlocation(name, tmploc);
}

template <>
void parser::separating<context_server>(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end, bool serv)
{
    tmpserv = server();
    CURLWAIT(begin, end, true);
    if (begin == end || (begin->type != WORD && begin->content != A))
        exit (1);
    begin++;
    CURLWAIT(begin, end);
    while (begin != end && begin->type != CLOSECURL)
    {
        CURLWAIT(begin, end, true);
        if (begin->type == WORD && begin->content == "location")
            separating<context_location>(begin, end, serv);
        else if (begin->type == WORD)
            separating<simpledir>(begin, end, serv);
    }
    if (begin == end)
        exit(0);
    begin++;
    CURLWAIT(begin, end, true);
    servers.push_back(tmpserv);
}

std::vector<server> parser::lexer_to_data(std::list<tokengen> lexer)
{
    std::list<tokengen>::iterator begin = lexer.begin();
    std::list<tokengen>::iterator end = lexer.end();
    while (begin != end)
        separating<context_server>(begin, end);
    if (servers.size() == 0)
        exit (1);
    return servers;
}

parser::~parser()
{
    config.close();
}
