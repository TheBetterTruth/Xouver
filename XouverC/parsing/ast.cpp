#include "ast.h"
#include "opcodes.h"

#include <vector>
#include "codegen/iscope.h"

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

const void ASTFunc::gen(IScope& scope, std::vector<unsigned char>& out) const {
	expr->gen(scope, out);
}