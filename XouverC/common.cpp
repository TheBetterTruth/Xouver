#include "common.h"
#include <string>
#include <vector>
#include "codegen/type.h"
#include "codegen/iscope.h"
#include "parsing/ast.h"

std::vector<std::string> types;
Class* _currentClass;
const Type null("null");

bool match(const std::string src, const char _char) {
	return src.find(_char) != std::string::npos;
}

bool match(const std::string src, const std::string str) {
	return src.find(str) != std::string::npos;
}

void write_bytes(const void* val, const size_t size, std::vector<unsigned char>& out) {
	unsigned char* buff = (unsigned char*) static_cast<const unsigned char*>(val);

	for (unsigned char* p = buff; p < buff + size; p++) {
		out.push_back(*p);
	}
}

const Type type_from(const std::unique_ptr<const ASTExpr>& expr) {
	const ASTExpr* last = static_cast<const ASTExpr*>(expr.get());

	while (last->type == ASTType::A_ACCESS) {
		const ASTAccess* access = static_cast<const ASTAccess*>(last);

		if (access->left->type == ASTType::A_ARRAY || access->left->type == ASTType::A_CALL)
			throw std::exception();

		last = static_cast<const ASTExpr*>(access->right.get());
	}


	if (last->type == ASTType::A_ARRAY) {
		if (last->type != ASTType::A_IDENT)
			throw std::exception();
	}
	else if (last->type != ASTType::A_IDENT)
		throw std::exception();
	std::string tname = expr->string();

	if (tname == "int" || tname == "Integer")
		tname = "Xouver.Integer";
	else if (tname == "float" || tname == "Float")
		tname = "Xouver.Float";
	else if (tname == "char" || tname == "Char")
		tname = "Xouver.Char";
	else if (tname == "bool" || tname == "Bool")
		tname = "Xouver.Bool";
	else if (tname == "string" || tname == "String")
		tname = "Xouver.String";

	return Type(tname);
}

const Type type_of(IScope& scope, const std::unique_ptr<const ASTExpr>& expr) {
	if (expr->type == ASTType::A_INT)
		return Type("Xouver.Integer");
	else if (expr->type == ASTType::A_FLOAT)
		return Type("Xouver.Float");
	else if (expr->type == ASTType::A_CHAR)
		return Type("Xouver.Char");
	else if (expr->type == ASTType::A_BOOL)
		return Type("Xouver.Bool");
	else if (expr->type == ASTType::A_STRING)
		return Type("Xouver.String");
	else if (expr->type == ASTType::A_IDENT)
		return scope.typeOf(static_cast<const ASTIdent*>(expr.get())->value);
	else if (expr->type == ASTType::A_BINARY)
		return type_of(scope, static_cast<const ASTBinary*>(expr.get())->lval);
}

const Class& currentClass() {
	return *_currentClass;
}