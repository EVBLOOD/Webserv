/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:58:39 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/12 22:30:37 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "tokengen.hpp"

enum func{
    setport,
    setservername,
    setmap,
    setroot,
    setclient_max_body_size,
    setlocation,
    setallow_methods,
    setfastcgi_pass,
    setindex,
    setreturn,
    setautoindex,
    setupload_enable,
    setupload_store
};

class location
{
    private:
        std::vector<std::string> index;
        std::map<int, std::string> ret_rn;
        bool autoindex;
        std::vector<std::string> allow_methods;
        std::string fastcgi_pass;
        bool upload_enable;
        std::string upload_store;
    public:
        location();
        ~location();
        template<int>
            void set(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end); // turn this to refrence
        // template<>
        // void set<setallow_methods>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
        // template<>
        //     void set<setfastcgi_pass>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
        // template<>
        //     void set<setindex>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
        // template<>
        //     void set<setreturn>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
        // template<>
        //     void set<setautoindex>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
        // template<>
        //     void set<setupload_enable>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
        // template<>
        //     void set<setupload_store>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
        void setallow_methods(std::string x);
        void setfastcgi_pass(std::string x);
        void setindex(std::string x);
        void setreturn(std::string x);
        void setautoindex(std::string x);
        void setupload_enable(std::string x);
        void setupload_store(std::string x);
};

template<int>
    void location::set(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
{
    std::cout << "what a funny joke\n";
}

template<>
    void location::set<setallow_methods>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
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
    if (tmp == "GET" || tmp == "POST"  || tmp == "DELETE")
        allow_methods.push_back(tmp);
    else
        exit (1); // alo alo
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        set<setallow_methods>(big, end);
}
template<>
    void location::set<setfastcgi_pass>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
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
    fastcgi_pass = tmp;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo alo
}
template<>
    void location::set<setindex>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
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
    allow_methods.push_back(tmp);
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        set<setindex>(big, end);
}
template<>
    void location::set<setreturn>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
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
    ret_rn.insert(std::make_pair(status, tmp));
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo
}
template<>
    void location::set<setautoindex>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
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
    if (tmp == 'on')
        autoindex = true;
    else if (tmp == 'off')
        autoindex = false;
    else
        exit (1); // error again!
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo
}
template<>
    void location::set<setupload_enable>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
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
    if (tmp == 'on')
        upload_enable = true;
    else if (tmp == 'off')
        upload_enable = false;
    else
        exit (1); // error again!
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo
}
template<>
    void location::set<setupload_store>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
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
    upload_store = tmp;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1); // alo alo
}
location::location()
{
    
}

location::~location()
{
    
}