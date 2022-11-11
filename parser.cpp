/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:04:48 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/11 18:43:55 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "tokengen.hpp"
#include <iterator>
#include <list>
#include <vector>
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
std::list<tokengen>::iterator CURLWAIT(std::list<tokengen>::iterator x, std::list<tokengen>::iterator end)
{
    while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
        x++;
    if (x == end || x->type != OPENCURL)
        exit (1); // you mad bro?
    x++;
    while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
        x++;
    if (x == end)
        exit (0); // lets go
    return (x);
}
enum KEYEGN{
  context_server,
  context_location,
  simpledir
};
template <int>
void separating(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end)
{
}

template <>
void separating<context_server>(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end)
{
    CURLWAIT(begin, end);
    while (begin != end && begin->type != CLOSECURL)
    {
        
        begin++;
    }
    if (begin == end)
        exit(0); // no end bro?
}

template <>
void separating<context_location>(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end)
{
    CURLWAIT(begin, end);
    while (begin != end && begin->type != CLOSECURL)
    {
        separating<simpledir>(begin, end);
        if (begin == end)
            exit (); // bruh are u seruios
        begin++;
    }
    if (begin == end)
        exit(0); // no end bro?
}

// ------------------------------
//  SEMICOLONS
//  OPENCURL
//  CLOSECURL
//  WHITESPACE
//  COLON
//  ENDOFLINE
//  COMMENT
// ------------------------------
//  WORD
//  QUOTES
// ------------------------------
template <>
void separating<simpledir>(std::list<tokengen>::iterator &begin, std::list<tokengen>::iterator &end, int) // I may need somewhere to store this data!
{
    bool multi = true;
    // (add this begin to your class and check if it's multuple)
    bool one = false;
    if (begin == end)
        exit (1); // yoou mad bro?
    while (begin != end && begin->type != SEMICOLONS)
    {
        while (begin != end && begin->type == WHITESPACE)
            begin++;
        if (begin == end || begin->type != WORD || begin->type != QUOTES)
            exit (1); // hehe
        if (multi == false && one)
            exit (0); // bro you mad again?
        // add this one to it's proper place
        // if () //else if () // else if () // else if () // or just throw an error! 
        one = true;
        begin++;
    }
}

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
            while (begin != end && begin->type != CLOSECURL)
            {
                if (begin->type == WORD && begin->content == B)
                {
                    
                }
            }
            if (begin == end)
                exit (0); // no closed curl
               
        }
        if (begin != end)
            begin++;
    }
    if (data.size() == 0)
        exit (1); // throw an error bro no data here 
    return NULL;
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
