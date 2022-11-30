/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sakllam <sakllam@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 18:04:29 by sakllam           #+#    #+#             */
/*   Updated: 2022/11/15 18:55:40 by sakllam          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "parser.hpp"
#include "tokengen.hpp"

int main() {
    std::cout << " -------------------------- BEGIN "
                 "---------------------------------- \n";
    parser file("conf");
    std::vector<std::string> type{"OPENCURL",   "CLOSECURL", "SEMICOLONS",
                                  "WHITESPACE", "WORD",      "QUOTES",
                                  "ENDOFLINE",  "COMMENT"};
    std::cout << " -------------------------- begin gen "
                 "---------------------------------- \n";
    std::list<tokengen> x = file.generate();
    std::cout << " -------------------------- end gen "
                 "---------------------------------- \n";
    std::list<tokengen>::iterator begin = x.begin();
    std::list<tokengen>::iterator end = x.end();
    while (begin != end) {
        std::cout << type[begin->type] << "   <--------->  " << begin->content
                  << "\n";
        begin++;
    }
    std::cout << " -------------------------- begin data "
                 "---------------------------------- \n";
    std::vector<server> xx = file.lexer_to_data(x);
    std::cout << " -------------------------- end data "
                 "---------------------------------- \n";
    std::cout << " -------------------------- END "
                 "---------------------------------- \n";
    std::cout << "count " << xx.size() << "\n";
}
