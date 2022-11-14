/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:04:29 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/14 12:54:02 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.hpp"
#include "tokengen.hpp"
#include <vector>

int main()
{
    parser file("conf");
    std::vector<std::string> type {"OPENCURL", "CLOSECURL", "SEMICOLONS", "WHITESPACE", "WORD", "QUOTES", "COLON", "ENDOFLINE", "COMMENT"};
    puts("--------------------------------------- token begin ----------------------------------------------------------");
    std::list<tokengen> x = file.generate();
    puts("--------------------------------------- token end ----------------------------------------------------------");
    std::list<tokengen>::iterator begin = x.begin();
    std::list<tokengen>::iterator end = x.end();
    while (begin != end)
    {
        std::cout << type[begin->type] << " " << begin->content << "\n"; 
        begin++;
    }
    std::vector<server> xx = file.lexer_to_data(x);
    std::cout << "DONE   " << "\n";
}