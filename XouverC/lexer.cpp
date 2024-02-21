#include "lexer.h"

lexer::lexer(const std::string _Code) : code(_Code), pos(0) {
    curreCHAR_NODE = code[pos];
}

void lexer::advance() {
    pos++;

    if (pos < code.size()) {
        curreCHAR_NODE = code[pos];
    }
    else {
        curreCHAR_NODE = '\0';
    }
}

void lexer::skip_whitespace() {
    while (curreCHAR_NODE != '\0' && isspace(curreCHAR_NODE)) {
        advance();
    }
}

std::string lexer::get_ident() {
    std::string res;

    while (curreCHAR_NODE != '\0' && isalpha(curreCHAR_NODE)) {
        res += curreCHAR_NODE;
        advance();
    }

    return res;
}

std::string lexer::get_num() {
    std::string res;

    while (isdigit(curreCHAR_NODE)) {
        res += curreCHAR_NODE;
        advance();
    }

    if (curreCHAR_NODE == '.') {
        res += curreCHAR_NODE;
        advance();

        while (curreCHAR_NODE != '\0' && isdigit(curreCHAR_NODE)) {
            res += curreCHAR_NODE;
            advance();
        }
    }

    return res;
}

token lexer::get_next_token() {
    while (curreCHAR_NODE != '\0') {
        if (isspace(curreCHAR_NODE)) {
            skip_whitespace();
            continue;
        }
        else if (isalpha(curreCHAR_NODE)) {
            std::string ident = get_ident();

            if (ident == "var") {
                return token(TT_VAR, ident);
            }
            else if (ident == "func") {
                return token(TT_FUNC, ident);
            }
            else if (ident == "if") {
                return token(TT_IF, ident);
            }
            else if (ident == "while") {
                return token(TT_WHILE, ident);
            }
            else if (ident == "for") {
                return token(TT_FOR, ident);
            }
            else if (ident == "init") {
                return token(TT_INIT, ident);
            }
            else {
                return token(TT_IDENT, ident);
            }
        }
        else if (isdigit(curreCHAR_NODE)) {
            std::string num = get_num();
            return token(TT_NUM, num);
        }
        else if (curreCHAR_NODE == '+') {
            advance();
            return token(TT_PLUS, "+");
        }
        else if (curreCHAR_NODE == '-') {
            advance();
            return token(TT_MINUS, "-");
        }
        else if (curreCHAR_NODE == '*') {
            advance();
            return token(TT_MUL, "*");
        }
        else if (curreCHAR_NODE == '/') {
            advance();
            return token(TT_DIV, "/");
        }
        else if (curreCHAR_NODE == '=') {
            advance();

            if (curreCHAR_NODE == '=') {
                advance();
                return token(TT_COMPARE, "==");
            }
            return token(TT_ASSIGN, "=");
        }
        else if (curreCHAR_NODE == '>') {
            advance();

            if (curreCHAR_NODE == '=') {
                advance();
                return token(TT_COMPARE, ">=");
            }
            return token(TT_COMPARE, ">");
        }
        else if (curreCHAR_NODE == '<') {
            advance();

            if (curreCHAR_NODE == '=') {
                advance();
                return token(TT_COMPARE, "<=");
            }
            return token(TT_COMPARE, "<");
        }
        else if (curreCHAR_NODE == '!') {
            advance();

            if (curreCHAR_NODE == '=') {
                advance();
                return token(TT_COMPARE, "!=");
            }
            return token(TT_NOT, "!");
        }
        else if (curreCHAR_NODE == ';') {
            advance();
            return token(TT_SEMICOLON, ";");
        }
        else if (curreCHAR_NODE == ':') {
            advance();
            return token(TT_COLON, ":");
        }
        else if (curreCHAR_NODE == '.') {
            advance();
            return token(TT_PERIOD, ".");
        }
        else if (curreCHAR_NODE == ',') {
            advance();
            return token(TT_COMMA, ",");
        }
        else if (curreCHAR_NODE == '(') {
            advance();
            return token(TT_LPAREN, "(");
        }
        else if (curreCHAR_NODE == ')') {
            advance();
            return token(TT_RPAREN, ")");
        }
        else if (curreCHAR_NODE == '[') {
            advance();
            return token(TT_LBRACK, "[");
        }
        else if (curreCHAR_NODE == ']') {
            advance();
            return token(TT_RBRACK, "]");
        }
        else if (curreCHAR_NODE == '{') {
            advance();
            return token(TT_LBRACE, "{");
        }
        else if (curreCHAR_NODE == '}') {
            advance();
            return token(TT_RBRACE, "}");
        }
    }

    return token(TT_EOF, "");
}
