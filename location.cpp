/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/13 13:33:40 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/15 16:42:56 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "location.hpp"

void CURLWAIT(std::list<tokengen>::iterator &x, std::list<tokengen>::iterator &end, bool loc)
{
    while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
        x++;
    if (loc)
        return ;
    if (x == end || x->type != OPENCURL)
        exit (1);
    x++;
    while (x != end && (x->type == WHITESPACE || x->type == COMMENT || x->type == ENDOFLINE))
        x++;
    if (x == end)
        exit (1);
}

// template<int>
//     void location::set(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
// {
//     std::cout << "what a funny joke\n";
// }

template<>
    void location::set<setallow_methods>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1);
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    if (tmp == "GET" || tmp == "POST"  || tmp == "DELETE")
        allow_methods.push_back(tmp);
    else
        exit (1);
    big++;
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        this->set<setallow_methods>(big, end);
    if (big == end)
        exit (0);
    big++;
}
template<>
    void location::set<setfastcgi_pass>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1);
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    fastcgi_pass = tmp;
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++;
}
template<>
    void location::set<setindex>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
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
    big++;
    CURLWAIT(big, end, true);
    if (big != end && big->type != SEMICOLONS)
        set<setindex>(big, end);
    if (big == end)
        exit (0);
    big++;
}
template<>
    void location::set<setreturn>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    std::string tmp;
    std::cout << big->content << "\n";
    big++;
    CURLWAIT(big, end, true);
    if (big == end || (big->type != WORD && big->type != QUOTES))
        exit (1); // alo alo
    if (big->type == QUOTES)
        tmp = big->content.substr(1, big->content.length() - 2);
    else
        tmp = big->content;
    for (int i = 0; tmp[i]; i++)
        if (std::isdigit(tmp[i]) == false)
            exit (1);
    int status;
    std::stringstream x;
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
    ret_rn.insert(std::make_pair(status, tmp));
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++;
}
template<>
    void location::set<setautoindex>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
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
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++;
}
template<>
    void location::set<setupload_enable>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
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
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++;
}
template<>
    void location::set<setupload_store>(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
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
    big++;
    CURLWAIT(big, end, true);
    if (big == end || big->type != SEMICOLONS)
        exit (1);
    big++;
}
typedef void (location::*function_location)(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end);
void location::execute(int i,std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end)
{
    function_location funs[] = {&location::set<6>, &location::set<7>, &location::set<8>, &location::set<9>, &location::set<10>, &location::set<11>, &location::set<12>};
    (this->*funs[12 - i])(big, end);
}

location::location()
{
    
}

location::~location()
{
    
}

location::location(const location &lc)
{
    *this = lc;
}

location &location::operator=(const location &lc)
{
    if (this == &lc)
    {
        std::cerr << "wait what??? \n";
        exit (1);
    }
    index = lc.index;
    ret_rn = lc.ret_rn;
    autoindex = lc.autoindex;
    allow_methods = lc.allow_methods;
    fastcgi_pass = lc.fastcgi_pass;
    upload_enable = lc.upload_enable;
    upload_store = lc.upload_store;
    return *this;
}