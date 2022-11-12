/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:12:22 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/12 16:08:58 by sakllam          ###   ########.fr       */
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
            void setters(std::string x, ...);
        template<>
            void setters<setport>(std::string x, ...);
        template<>
            void setters<setservername>(std::string x, ...);
        template<>
            void setters<setmap>(std::string x, ...);
        template<>
            void setters<setroot>(std::string x, ...);
        template<>
            void setters<setlocation>(std::string x, ...);
        template<>
            void setters<setclient_max_body_size>(std::string x, ...);
        // void setport(std::string x);
        // void setservername(std::string x);
        // void setmap(std::string x, std::string y);
        // void setroot(std::string x);
        // void setlocation(std::string x, location y);
        // void setclient_max_body_size(std::string x);
};

server::server()
{
}

server::~server()
{
}
