/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/08 22:07:55 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/09 16:20:22 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <fstream>
#include <iostream>
class parser
{
    private:
        std::ifstream config;
        
    public:
        parser(const std::string &filename);
        // tokens  &generate();
        ~parser();
};

parser::parser(const std::string &filename)
{
    config.open(filename); // open file
    if (config.is_open() == false ) // protection mode: file opened?
    { // no ? okey
        std::cout << "Some respect please ! \n";
        exit(1);
        // else throw an error;
    }
    if (config.peek() == EOF) // protection mode: nothing in the file
    {// EOF > okey
        std::cout << "Empty ---> \n";
        exit (1);
        // else throw an error;
    }
    // generate tokens! 
}

parser::~parser()
{
    config.close();
}
