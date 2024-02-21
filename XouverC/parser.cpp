#include "parser.h"
#include <memory>
#include <map>

parser::parser(const std::string _ClassName, const std::string _ClassPath, const std::string _InputCode) :
    _lexer(_InputCode)
{
    current_token = _lexer.get_next_token();
}

std::unique_ptr<class_node> parser::parse() {
    auto prog = program("Xouver", "Test");

    prog->print();
    return prog;
}

void parser::eat(const token_type _Type) {
    if (current_token.type == _Type)
        current_token = _lexer.get_next_token();
    else
        throw std::runtime_error("Unexpected token: " + current_token.value);
}

std::unique_ptr<class_node> parser::program(std::string _ClassName, std::string _ClassPath) {
    std::vector<node_ptr> body;

    while (current_token.type != TT_EOF) {
        if (current_token.type == TT_VAR) {
            body.push_back(var_decl());
        }
        else if (current_token.type == TT_INIT) {
            body.push_back(class_init());
        }
        else if (current_token.type == TT_FUNC) {
            body.push_back(func_decl());
        }
        else {
            throw std::runtime_error("Unexpected token: " + current_token.value);
        }
    }

    return std::make_unique<class_node>(_ClassName, _ClassPath, std::move(body));
}
node_ptr parser::class_init() {
    eat(TT_INIT);
    eat(TT_LPAREN);

    std::vector<std::string> params;

    while (current_token.type != TT_RPAREN) {
        params.push_back(current_token.value);
        eat(TT_IDENT);

        if (current_token.type != TT_RPAREN)
            eat(TT_COMMA);
    }
    eat(TT_RPAREN);

    return std::make_unique<func_decl_node>("init", params, statement());
}
node_ptr parser::func_decl() {
    eat(TT_FUNC);
    std::string name = current_token.value;
    eat(TT_IDENT);
    eat(TT_LPAREN);

    std::vector<std::string> params;
    while (current_token.type != TT_RPAREN) {
        params.push_back(current_token.value);
        eat(TT_IDENT);

        if (current_token.type != TT_RPAREN) {
            eat(TT_COMMA);
        }
    }
    eat(TT_RPAREN);

    return std::make_unique<func_decl_node>(name, params, statement());
}
node_ptr parser::statement() {
    if (current_token.type == TT_LBRACE) {
        eat(TT_LBRACE);
        std::vector<node_ptr> statements;

        while (current_token.type != TT_RBRACE)
            statements.push_back(statement());
        eat(TT_RBRACE);

        return std::make_unique<block_node>(std::move(statements));
    }
    if (current_token.type == TT_VAR) {
        return var_decl();
    }
    else {
        return cond();
    }
}
node_ptr parser::if_statement() {
    eat(TT_IF);
    eat(TT_LPAREN);

    auto condition = cond();

    eat(TT_RPAREN);

    return std::make_unique<if_node>(std::move(condition), statement());
}
node_ptr parser::while_statement() {
    eat(TT_WHILE);
    eat(TT_LPAREN);

    auto condition = cond();

    eat(TT_RPAREN);

    return std::make_unique<while_node>(std::move(condition), statement());
}
node_ptr parser::for_statement() {
    eat(TT_FOR);

    // Handle for-expressions

    return nullptr;
}

node_ptr parser::var_decl() {
    eat(TT_VAR);
    std::string ident = current_token.value;
    eat(TT_IDENT);
    eat(TT_ASSIGN);

    return std::make_unique<var_decl_node>(ident, cond());
}

node_ptr parser::cond() {
    node_ptr node = expr();

    while (current_token.type == TT_COMPARE) {
        std::string op = current_token.value;

        eat(TT_COMPARE);

        node = std::make_unique<condition_node>(std::move(node), op, expr());
    }

    return node;
}

node_ptr parser::expr() {
    node_ptr node = term();

    while (current_token.type == TT_PLUS || current_token.type == TT_MINUS) {
        char op = current_token.value[0];

        if (op == '+')
            eat(TT_PLUS);
        else
            eat(TT_MINUS);
        
        node = std::make_unique<binop_node>(std::move(node), op, term());
    }

    return node;
}

node_ptr parser::term() {
    node_ptr node = fac();

    while (current_token.type == TT_MUL || current_token.type == TT_DIV) {
        char op = current_token.value[0];

        if (op == '*')
            eat(TT_MUL);
        else
            eat(TT_DIV);
        
        node = std::make_unique<binop_node>(std::move(node), op, fac());
    }

    return node;
}

node_ptr parser::fac() {
    node_ptr expr = primary();

    
    if (current_token.type == TT_PERIOD) {
        eat(TT_PERIOD);

        return std::make_unique<member_node>(std::move(expr), fac());
    }
    else if (current_token.type == TT_ASSIGN) {
        eat(TT_ASSIGN);

        return std::make_unique<assign_node>(std::move(expr), this->expr());
    }

    return expr;
}

node_ptr parser::primary() {
    if (current_token.type == TT_IDENT) {
        std::string ident = current_token.value;

        eat(TT_IDENT);
        node_ptr res = std::make_unique<ident_node>(ident);

        while (current_token.type == TT_LPAREN || current_token.type == TT_LBRACK) {
            if (current_token.type == TT_LPAREN) {
                eat(TT_LPAREN);

                std::vector<node_ptr> args;
                
                while (current_token.type != TT_RPAREN) {
                    args.push_back(cond());

                    if (current_token.type != TT_RPAREN)
                        eat(TT_COMMA);
                }
                eat(TT_RPAREN);

                res = std::make_unique<call_node>(std::move(res), std::move(args));
            }
            else if (current_token.type == TT_LBRACK) {
                eat(TT_LBRACK);
                node_ptr elem = expr();
                eat(TT_RBRACK);

                res = std::make_unique<element_node>(std::move(res), std::move(elem));
            }
        }
        
        return res;
    }
    else if (current_token.type == TT_NUM) {
        std::string num = current_token.value;
        eat(TT_NUM);
        return std::make_unique<num_node>(stod(num));
    }
    else if (current_token.type == TT_PLUS || current_token.type == TT_MINUS || current_token.type == TT_NOT) {
        char op = current_token.value[0];

        eat(current_token.type);

        return std::make_unique<unop_node>(op, fac());
    }
    else if (current_token.type == TT_LPAREN) {
        eat(TT_LPAREN);
        node_ptr expr = this->expr();
        eat(TT_RPAREN);

        return expr;
    }
    else {
        throw std::runtime_error("Invalid token: " + current_token.value);
    }
}
