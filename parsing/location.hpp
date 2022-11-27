/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:58:39 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/18 12:59:32 by sakllam          ###   ########.fr       */
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

enum func {
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

class location {
public:
    std::vector <std::string> index;
    std::map<int, std::string> ret_rn;
    bool autoindex;
    std::vector <std::string> allow_methods;
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
};

void CURLWAIT(std::list<tokengen>::iterator &x, std::list<tokengen>::iterator &end, bool loc = false);
