/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:04:29 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/14 19:03:42 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "tokengen.hpp"
#include <vector>

int main()
{
    parser file("conf");
    std::vector<std::string> type {"OPENCURL", "CLOSECURL", "SEMICOLONS", "WHITESPACE", "WORD", "QUOTES", "ENDOFLINE", "COMMENT"};
    std::list<tokengen> x = file.generate();
    std::list<tokengen>::iterator begin = x.begin();
    std::list<tokengen>::iterator end = x.end();
    while (begin != end)
    {
        std::cout << type[begin->type] << " " << begin->content << "\n"; 
        begin++;
    }
    std::cout << " -------------------------- BEGIN ---------------------------------- \n";
    std::vector<server> xx = file.lexer_to_data(x);
    std::cout << " -------------------------- DONE ---------------------------------- \n";
}