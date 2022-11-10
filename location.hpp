/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 22:58:39 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/10 22:59:00 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <vector>
#include <map>

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
        void setallow_methods(std::string x);
        void setfastcgi_pass(std::string x);
        void setindex(std::string x);
        void setreturn(std::string x);
        void setautoindex(std::string x);
        void setupload_enable(std::string x);
        void setupload_store(std::string x);
};

location::location()
{
    
}

location::~location()
{
    
}