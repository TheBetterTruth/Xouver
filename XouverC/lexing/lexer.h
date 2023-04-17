#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include <memory>
#include <istream>
#include "token.h"

std::vector<std::unique_ptr<const Token>> lex(std::istream& is);

#endif