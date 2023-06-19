#include "ast.h"
#include "opcodes.h"

#include <vector>
#include "codegen/iscope.h"
#include "common.h"

const void ASTInt::gen(IScope& scope, std::vector<unsigned char>& out) const {
	out.push_back(OP_CLOAD);
	scope.getConst(value, out);
}

const void ASTBinary::gen(IScope& scope, std::vector<unsigned char>& out) const {
	lval->gen(scope, out);
	rval->gen(scope, out);

	if (op == '+')
		out.push_back(OP_ADD);
	else if (op == '-')
		out.push_back(OP_SUB);
	else if (op == '*')
		out.push_back(OP_MUL);
	else if (op == '/')
		out.push_back(OP_DIV);
}

const void ASTIdent::gen(IScope& scope, std::vector<unsigned char>& out) const {
	scope.get(value, out);
}

const void ASTCall::gen(IScope& scope, std::vector<unsigned char>& out) const {
	if (callee->type == ASTType::A_IDENT) {
		std::vector<Type> paramTypes;

		for (auto& a : args) {
			Type t = type_of(scope, a);
			paramTypes.push_back(t);
			a->gen(scope, out);
		}
		scope.call(static_cast<const ASTIdent*>(callee.get())->value, paramTypes, out);
	}
}

const void ASTAssign::gen(IScope& scope, std::vector<unsigned char>& out) const {
	if (dest->type == ASTType::A_VAR) {
		const ASTVar* var = static_cast<const ASTVar*>(dest.get());
		value->gen(scope, out);

		if (var->name->type == ASTType::A_IDENT) {
			scope.put(type_from(var->varType), static_cast<const ASTIdent*>(var->name.get())->value, out);
		}
		else throw std::exception();
	}
	else if (dest->type == ASTType::A_IDENT) {
		const ASTIdent* ident = static_cast<const ASTIdent*>(dest.get());
		value->gen(scope, out);

		const Type t = type_of(scope, dest);
		scope.put(t, ident->value, out);
	}
	else throw std::exception();
}

const void ASTBlock::gen(IScope& scope, std::vector<unsigned char>& out) const {
	Scope s(scope, {});

	for (auto& e : exprs)
		e->gen(s, out);
}

const void ASTFunc::gen(IScope& scope, std::vector<unsigned char>& out) const {
	if (expr->type == ASTType::A_BLOCK)
		for (auto& e : static_cast<const ASTBlock*>(expr.get())->exprs)
			e->gen(scope, out);
	else expr->gen(scope, out);
}

const void ASTNew::gen(IScope& scope, std::vector<unsigned char>& out) const {
	if (expr->type == ASTType::A_ARRAY) {

	}
}