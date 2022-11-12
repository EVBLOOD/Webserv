/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:58:39 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/12 19:17:45 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
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
            void set(std::string x);
        template<>
        void set<setallow_methods>(std::string x);
        template<>
            void set<setfastcgi_pass>(std::string x);
        template<>
            void set<setindex>(std::string x);
        template<>
            void set<setreturn>(std::string x);
        template<>
            void set<setautoindex>(std::string x);
        template<>
            void set<setupload_enable>(std::string x);
        template<>
            void set<setupload_store>(std::string x);
        void setallow_methods(std::string x);
        void setfastcgi_pass(std::string x);
        void setindex(std::string x);
        void setreturn(std::string x);
        void setautoindex(std::string x);
        void setupload_enable(std::string x);
        void setupload_store(std::string x);
};

template<int>
    void location::set(std::string x)
{
    std::cout << "what a funny joke\n";
}
template<>
    void location::set<setallow_methods>(std::string x)
{
    if (x == "GET" || x == "POST"  || x == "DELETE")
    {
        allow_methods.push_back(x);
        return;
    }
    exit (1); // allo 
}
template<>
    void location::set<setfastcgi_pass>(std::string x)
{
    fastcgi_pass = x;
}
template<>
    void location::set<setindex>(std::string x)
{
    index.push_back(x);
}
template<>
    void location::set<setreturn>(std::string x)
{
    // ret_rn.insert()
}
template<>
    void location::set<setautoindex>(std::string x)
{
    
}
template<>
    void location::set<setupload_enable>(std::string x)
{
    
}
template<>
    void location::set<setupload_store>(std::string x)
{
    
}
location::location()
{
    
}

location::~location()
{
    
}