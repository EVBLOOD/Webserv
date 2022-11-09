/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 22:07:55 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 18:08:38 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <cstdio>
#include <string>
#include <fstream>
#include <iostream>
#include <list>
#include "tokengen.hpp"
#include <cctype>
class parser
{
    private:
        std::ifstream config;
        
    public:
        parser(const std::string &filename);
        std::list<tokengen> generate();
        ~parser();
};
