#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include "lexing/token.h"
#include "ast.h"

enum Modifiers {
	M_PUB = 1,
	M_PRIV = 2,
	M_STAT = 4,
	M_NAT = 8
};

std::vector<std::unique_ptr<const ASTExpr>> parse(std::vector<std::unique_ptr<const Token>>* _tokens);

#endif