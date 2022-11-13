/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 13:33:40 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/13 13:34:25 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"


void CURLWAIT(std::list<tokengen>::iterator &x, std::list<tokengen>::iterator &end, bool loc = false)
{
    while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
        x++;
    if (loc)
        return ;
    if (x == end ||x->type != OPENCURL)
        exit (1); // you mad bro?
    x++;
    while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
        x++;
    if (x == end)
        exit (0); // lets go
}

// template<int>
//     void location::set(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
// {
//     std::cout << "what a funny joke\n";
// }

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
        this->set<setallow_methods>(big, end);
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
    if (tmp == "on")
        autoindex = true;
    else if (tmp == "off")
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
    if (tmp == "on")
        upload_enable = true;
    else if (tmp == "off")
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