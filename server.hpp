/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:12:22 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/12 22:53:44 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include <map>
#include "location.hpp"




class server
{
    private:
        // std::vector<std::string> index;
        std::vector<int> port;
        std::vector<std::string> server_name;
        std::map<int, std::string> error_page;
        std::string                 root;
        std::map<std::string, location> locations;
        unsigned long long client_max_body_size;
        
    public:
        server();
        ~server();
        template<int>
            void setters(std::list<tokengen>::iterator big, std::list<tokengen> end);
        template<>
            void setters<setport>(std::list<tokengen>::iterator big, std::list<tokengen> end);
        template<>
            void setters<setservername>(std::list<tokengen>::iterator big, std::list<tokengen> end);
        template<>
            void setters<setmap>(std::list<tokengen>::iterator big, std::list<tokengen> end);
        template<>
            void setters<setroot>(std::list<tokengen>::iterator big, std::list<tokengen> end);
        template<>
            void setters<setlocation>(std::list<tokengen>::iterator big, std::list<tokengen> end);
        template<>
            void setters<setclient_max_body_size>(std::list<tokengen>::iterator big, std::list<tokengen> end);
        // void setport(std::string x);
        // void setservername(std::string x);
        // void setmap(std::string x, std::string y);
        // void setroot(std::string x);
        void setlocation(std::string x, location y);
        // void setclient_max_body_size(std::string x);
};

void server::setlocation(std::string x, location y)
{
    locations.insert(std::make_pair(x, y)); // don't forget the path in the other side plz
}

template<int>
    void server::setters(std::list<tokengen>::iterator big, std::list<tokengen> end)
{
    std::cerr << "always dreaming";
}
template<>
    void server::setters<setport>(std::list<tokengen>::iterator big, std::list<tokengen> end)
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
    // iostream x = tmp;
    // if not working check if it will
    port.push_back(ports);
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo alo
}

template<>
    void server::setters<setservername>(std::list<tokengen>::iterator big, std::list<tokengen> end)
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
    void server::setters<setmap>(std::list<tokengen>::iterator big, std::list<tokengen> end)
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
    int status;
    // iostream x = tmp;
    // status = x;
    // and check for errors!
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
    void server::setters<setroot>(std::list<tokengen>::iterator big, std::list<tokengen> end)
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
// template<>
//     void setters<setlocation>(std::list<tokengen>::iterator big, std::list<tokengen> end); // I not yet sure how I'll be using this one
template<>
    void server::setters<setclient_max_body_size>(std::list<tokengen>::iterator big, std::list<tokengen> end)
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
    client_max_body_size.push_back(tmp);
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
