/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:12:22 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/14 12:24:49 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include <map>
#include "location.hpp"
#include "tokengen.hpp"
#include <list>

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
        server(const server &sv);
        server();
        ~server();
        server &operator=(const server &server);
        template<int>
            void setters(std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end);
        void setlocation(std::string x, location y);
        void execute(int i, std::list<tokengen>::iterator &big, std::list<tokengen>::iterator &end);
};
