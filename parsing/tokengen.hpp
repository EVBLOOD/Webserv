

#pragma once
#include <string>

enum Type {
    OPENCURL,
    CLOSECURL,
    SEMICOLONS,
    WHITESPACE,
    WORD,
    QUOTES,
    ENDOFLINE,
    COMMENT
};

struct tokengen {
    int type;
    std::string content;
    tokengen(int type);
    tokengen(int type, std::string content);
};
