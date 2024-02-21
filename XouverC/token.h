#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum token_type {
    TT_EOF,
    TT_NUM,
    TT_CHAR,
    TT_BOOL,
    TT_STRING,
    TT_IDENT,
    TT_PLUS,
    TT_MINUS,
    TT_MUL,
    TT_DIV,
    TT_ASSIGN,
    TT_VAR,
    TT_FUNC,
    TT_IF,
    TT_WHILE,
    TT_FOR,
    TT_INIT,
    TT_LPAREN,
    TT_RPAREN,
    TT_LBRACK,
    TT_RBRACK,
    TT_LBRACE,
    TT_RBRACE,
    TT_COLON,
    TT_SEMICOLON,
    TT_PERIOD,
    TT_COMMA,
    TT_COMPARE,
    TT_NOT
};

struct token {
    token_type type;
    std::string value;

    token(const token_type _Type, const std::string& _Value) :
        type(_Type), value(_Value) {}
    token() : type(TT_EOF) {}
};

#endif