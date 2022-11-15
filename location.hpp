/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:58:39 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/15 18:44:50 by sakllam          ###   ########.fr       */
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
    sethost,
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
        location(const location &lc);
        location();
        ~location();
        location &operator=(const location &lc);
        template<int>
            void set(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end); // turn this to refrence
        void execute(int i, std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end);
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

void CURLWAIT(std::list<tokengen>::iterator &x, std::list<tokengen>::iterator &end, bool loc = false);