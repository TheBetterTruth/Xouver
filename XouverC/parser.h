#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <memory>
#include "lexer.h"
#include "nodes.h"

class parser {
private:
    lexer _lexer;
    token current_token;

    void eat(const token_type _Type);
    std::unique_ptr<class_node> program(std::string _ClassName, std::string _ClassPath);
    node_ptr if_statement();
    node_ptr while_statement();
    node_ptr for_statement();
    node_ptr class_init();
    node_ptr func_decl();
    node_ptr statement();
    node_ptr var_decl();
    node_ptr cond();
    node_ptr expr();
    node_ptr term();
    node_ptr fac();
    node_ptr primary();
public:
    parser(const std::string _ClassName, const std::string _ClassPath, const std::string _InputCode);
    std::unique_ptr<class_node> parse();
};


#endif