#include "parser.hpp"
#include "tokengen.hpp"
#include <vector>
#include <map>


int main() {
//    std::cout << " -------------------------- BEGIN ---------------------------------- \n";
    parser file("conf2");
//    std::vector <std::string> type{"OPENCURL", "CLOSECURL", "SEMICOLONS", "WHITESPACE", "WORD", "QUOTES", "ENDOFLINE",
//                                   "COMMENT"};
//    std::cout << " -------------------------- begin gen ---------------------------------- \n";
    std::list <tokengen> x = file.generate();
//    stdtd::cout << " -------------------------- end gen ---------------------------------- \n";
//    std::list<tokengen>::iterator begin = x.begin();
//    std::list<tokengen>::iterator end = x.end();
//    while (begin != end) {
//        std::cout << type[begin->type] << "   <--------->  " << begin->content << "\n";
//        begin++;
//    }
//    std::cout << " -------------------------- begin data ---------------------------------- \n";
    std::vector <server> xx = file.lexer_to_data(x);
//    std::cout << " -------------------------- end data ---------------------------------- \n";
//    std::cout << " -------------------------- END ---------------------------------- \n";
//    std::cout << "count " << xx.size() << "\n";
    std::map<std::pair<int, std::string>, int> idea;
    int i = 0;
    for (auto x: xx) {
        for (auto p: x.port) {
            idea.insert(std::make_pair(std::make_pair(p, ""), i));
            for (auto n: x.server_name) {
                idea.insert(std::make_pair(std::make_pair(p, n), i));
            }
        }
        ++i;
    }
    for (auto x: idea) {
        std::cout << "server id " << x.second << " port " << x.first.first << " server name " << x.first.second << '\n';
    }
}
