#ifndef FUNCTION_H
#define FUNCTION_H

#include <string>
#include <vector>
#include <memory>
#include "type.h"
#include "var.h"
#include "parsing/ast.h"

struct Function {
	const std::string signature;
	const Type type;
	const std::string name;
	const std::vector<Var> params;
	const std::unique_ptr<const ASTExpr> expr;
	unsigned int ptr;

	Function(const std::string& signature, const Type type, const std::string& name, const std::vector<Var>& params, std::unique_ptr<const ASTExpr>& expr) :
		signature(signature), type(type), name(name), params(params), expr(std::move(expr)) {}
};

#endif