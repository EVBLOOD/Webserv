/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 13:33:03 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/15 16:51:19 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

void server::setlocation(std::string x, location y)
{
    locations.insert(std::make_pair(x, y)); // don't forget the path in the other side plz
}

template<>
    void server::setters<setport>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    int ports;
    for (int i = 0; tmp[i]; i++)
        if (isdigit(tmp[i]) == false) // TODO: check this
            exit (1);
    std::stringstream x;
    x << tmp;
    x >> ports;
    port.push_back(ports);
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++; // TODO: check if it's the end in the other side;
}

template<>
    void server::setters<setservername>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES) // TODO: check if the double quotes are double
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    server_name.push_back(tmp);
    big++;
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        setters<setservername>(big, end);
    if (big == end)
        exit (1); // TODO: check in the other side!
    big++;
}

template<>
    void server::setters<setmap>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    int status;
    std::stringstream x;


    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1);
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    for (int i = 0; tmp[i]; i++)
        if (isdigit(tmp[i]) == false) // TODO: for the port the number is limited and status
            exit (1);
    x << tmp;
    x >> status;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1);
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    error_page.insert(std::make_pair(status, tmp));
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++;
}

template<>
    void server::setters<setroot>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1);
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    root = tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++;
}

template<>
    void server::setters<setclient_max_body_size>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    unsigned long long size;
    std::stringstream x;
    

    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1);
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    for (int i = 0; tmp[i]; i++)
        if (isdigit(tmp[i]) == false)
            exit (1);
    x << tmp;
    x >> size; // TODO: maybe I'll make it count by Mb
    client_max_body_size = size; // TODO: read about this one and find out the real lims
    big++;
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
       exit (1);
    big++;
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