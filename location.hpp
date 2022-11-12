/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:58:39 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/12 22:05:06 by sakllam          ###   ########.fr       */
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
            void set(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end);
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
    // if (x == "GET" || x == "POST"  || x == "DELETE")
    // {
    //     allow_methods.push_back(x);
    //     return;
    // }
    exit (1); // allo 
}
template<>
    void location::set<setfastcgi_pass>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
{
    // fastcgi_pass = x;
}
template<>
    void location::set<setindex>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
{
    // index.push_back(x);
}
template<>
    void location::set<setreturn>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
{
    // ret_rn.insert()
}
template<>
    void location::set<setautoindex>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
{
    
}
template<>
    void location::set<setupload_enable>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
{
    
}
template<>
    void location::set<setupload_store>(std::list<tokengen>::iterator big, std::list<tokengen>::iterator end)
{
    
}
location::location()
{
    
}

location::~location()
{
    
}