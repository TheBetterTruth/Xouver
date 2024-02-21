#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <memory>
#include "token.h"

class lexer {
private:
    const std::string code;
    unsigned int pos = 0;
    char curreCHAR_NODE = '\0';

    void advance();
    void skip_whitespace();
    std::string get_ident();
    std::string get_num();
public:
    lexer(const std::string _Code);
    token get_next_token();
};

#endif