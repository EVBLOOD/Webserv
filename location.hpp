/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:58:39 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/13 13:34:21 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "tokengen.hpp"
#include <list>
#include <sstream>
#include <cctype>

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
        // void setallow_methods(std::string x);
        // void setfastcgi_pass(std::string x);
        // void setindex(std::string x);
        // void setreturn(std::string x);
        // void setautoindex(std::string x);
        // void setupload_enable(std::string x);
        // void setupload_store(std::string x);
};

