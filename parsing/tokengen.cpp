
#include "tokengen.hpp"

tokengen::tokengen(int type, std::string content)
    : type(type), content(content) {}

tokengen::tokengen(int type) : type(type), content(std::string()) {}
