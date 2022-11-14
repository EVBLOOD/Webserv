/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 13:33:03 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/14 12:39:30 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::setlocation(std::string x, location y)
{
    locations.insert(std::make_pair(x, y)); // don't forget the path in the other side plz
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


    // std::vector<std::pair<bool, std::string> > x;
    // x.push_back(std::make_pair(true, C));
    // x.push_back(std::make_pair(true, N));
    // x.push_back(std::make_pair(true, F));
    // x.push_back(std::make_pair(false, D));
    // x.push_back(std::make_pair(false, E));
    // // location
    // if (server)
    //     return x;
    // x.push_back(std::make_pair(true, I));
    // x.push_back(std::make_pair(false, K));
    // x.push_back(std::make_pair(false, H));
    // x.push_back(std::make_pair(false, J));
    // x.push_back(std::make_pair(false, G));
    // x.push_back(std::make_pair(false, L));
    // x.push_back(std::make_pair(false, M));
// typedef void (location::*function_location)(void);

// template<int>
//     void server::setters(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
// {
//     std::cout << "jokes" << "\n";
//     exit (1);
// }

template<>
    void server::setters<setport>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 1);
    else
        tmp = big->content;
    int ports;
    for (int i = 0; tmp[i]; i++)
        if (std::isdigit(tmp[i]) == false)
            exit (1); // bruuuuh 
    std::stringstream x;
    x << tmp;
    x >> ports;
    port.push_back(ports);
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo alo
}

template<>
    void server::setters<setservername>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 1);
    else
        tmp = big->content;
    server_name.push_back(tmp);
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        setters<setservername>(big, end);
}

template<>
    void server::setters<setmap>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
        std::string tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 1);
    else
        tmp = big->content;
    for (int i = 0; tmp[i]; i++)
        if (std::isdigit(tmp[i]) == false)
            exit (1); // bruuuuh 
    int status;
    std::stringstream x;
    x << tmp;
    x >> status;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 1);
    else
        tmp = big->content;
    error_page.insert(std::make_pair(status, tmp));
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo
}

template<>
    void server::setters<setroot>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 1);
    else
        tmp = big->content;
    root = tmp;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo
}

template<>
    void server::setters<setclient_max_body_size>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 1);
    else
        tmp = big->content;
    unsigned long long size;
    for (int i = 0; tmp[i]; i++)
        if (std::isdigit(tmp[i]) == false)
            exit (1); // bruuuuh 
    std::stringstream x;
    x << tmp;
    x >> size;
    client_max_body_size = size;
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        setters<setclient_max_body_size>(big, end);
}


server::server()
{
}

server::~server()
{
}


typedef void (server::*function_server)(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end);

void server::execute(int i,std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    function_server funs[] = {&server::setters<0>, &server::setters<1>, &server::setters<2>, &server::setters<3>, &server::setters<4>};
    (this->*funs[i])(big, end);
}

server::server(const server &sv)
{
    *this = sv;
}

server &server::operator=(const server &sv)
{
    if (this == &sv)
    {
        std::cerr << "this was unexpected!";
        exit (1);
    }
    port = sv.port;
    server_name = sv.server_name;
    error_page = sv.error_page;
    root = sv.root;
    locations = sv.locations;
    client_max_body_size= sv.client_max_body_size;
    return *this;
}