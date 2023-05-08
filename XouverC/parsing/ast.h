#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <sstream>
#include <iostream>
#include "codegen/iscope.h"
#include "codegen/scope.h"

enum ASTType {
	A_INT,
	A_FLOAT,
	A_CHAR,
	A_BOOL,
	A_STRING,
	A_PAREN,
	A_BINARY,
	A_UNARY,
	A_BLOCK,
	A_ACCESS,
	A_CALL,
	A_ARRAY,
	A_VAR,
	A_ASSIGN,
	A_IDENT,
	A_DEF,
	A_FUNC,
};

struct ASTExpr {
	const ASTType type;
	const int line;

	ASTExpr(const ASTType type, const int& line) :
		type(type), line(line) {}
	virtual const std::string string() const { return ""; }
	virtual const void gen(IScope& scope, std::vector<unsigned char>& out) const {}
};
template <typename T>
struct ASTConst : ASTExpr {
	T value;

	ASTConst(const ASTType type, const int& line, T value) :
		ASTExpr(type, line), value(value) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << value;

		return ss.str();
	}
};
struct ASTInt : ASTConst<const int> {
	ASTInt(const int& line, const int& value) :
		ASTConst(ASTType::A_INT, line, value) {}
	const void gen(IScope& scope, std::vector<unsigned char>& out) const override;
};
struct ASTFloat : ASTConst<const float> {
	ASTFloat(const int& line, const float& value) :
		ASTConst(ASTType::A_FLOAT, line, value) {}
};
struct ASTChar : ASTConst<const unsigned char> {
	ASTChar(const unsigned int& line, const unsigned char& value) :
		ASTConst(ASTType::A_CHAR, line, value) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << '\'' << value << '\'';

		return ss.str();
	}
};
struct ASTBool : ASTConst<const bool> {
	ASTBool(const unsigned int& line, const bool& value) :
		ASTConst(ASTType::A_BOOL, line, value) {}
};
struct ASTString : ASTConst<const std::string> {
	ASTString(const unsigned int& line, const std::string& value) :
		ASTConst(ASTType::A_STRING, line, value) {}
	const std::string string() const override {
		return "\"" + value + "\"";
	}
};
struct ASTBinary : ASTExpr {
	const std::unique_ptr<const ASTExpr> lval;
	const unsigned char op;
	const std::unique_ptr<const ASTExpr> rval;

	ASTBinary(const unsigned int& line, std::unique_ptr<const ASTExpr>& lval, const unsigned char& op, std::unique_ptr<const ASTExpr>& rval) :
		ASTExpr(ASTType::A_BINARY, line), lval(std::move(lval)), op(op), rval(std::move(rval)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << "BinExpr(" << lval->string() << ' ' << op << ' ' << rval->string() << ')';

		return ss.str();
	}
	const void gen(IScope& scope, std::vector<unsigned char>& out) const override;
};

enum UnaryOP {
	U_PLUS,
	U_MINUS,
	U_INC,
	U_DEC
};
struct ASTUnary : ASTExpr {
	const UnaryOP op;
	const std::unique_ptr<const ASTExpr> expr;

	ASTUnary(const unsigned int& line, const UnaryOP& op, std::unique_ptr<const ASTExpr>& expr) :
		ASTExpr(ASTType::A_UNARY, line), op(op), expr(std::move(expr)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << (op == U_PLUS ? "+" : op == U_MINUS ? "-" : op == U_INC ? "++" : "--");
		ss << expr->string();

		return ss.str();
	}
};

struct ASTBlock : ASTExpr {
	const std::vector<std::unique_ptr<const ASTExpr>> exprs;

	ASTBlock(const unsigned int& line, std::vector<std::unique_ptr<const ASTExpr>>& exprs) :
		ASTExpr(ASTType::A_BLOCK, line), exprs(std::move(exprs)) {}
	const std::string string() const override {
		std::stringstream ss;

		ss << "{\n";

		for (auto& e : exprs)
			ss << e->string() << '\n';

		ss << "}";

		return ss.str();
	}
	const void gen(IScope& scope, std::vector<unsigned char>& out) const override;
};

struct ASTIdent : ASTExpr {
	const std::string value;

	ASTIdent(const unsigned int& line, const std::string& value) :
		ASTExpr(ASTType::A_IDENT, line), value(value) {}
	const std::string string() const override {
		return value;
	}
	const void gen(IScope& scope, std::vector<unsigned char>& out) const override;
};
struct ASTAccess : ASTExpr{
	const std::unique_ptr<const ASTExpr> left;
	const std::unique_ptr<const ASTExpr> right;

	ASTAccess(const unsigned int& line, std::unique_ptr<const ASTExpr>& left, std::unique_ptr<const ASTExpr>& right) :
		ASTExpr(ASTType::A_ACCESS, line), left(std::move(left)), right(std::move(right)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << left->string() << '.' << right->string();

		return ss.str();
	}
};
struct ASTCall : ASTExpr {
	const std::unique_ptr<const ASTExpr> callee;
	const std::vector<std::unique_ptr<const ASTExpr>> args;
	
	ASTCall(const unsigned int& line, std::unique_ptr<const ASTExpr>& callee, std::vector<std::unique_ptr<const ASTExpr>>& args) :
		ASTExpr(ASTType::A_CALL, line), callee(std::move(callee)), args(std::move(args)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << callee->string() << '(';

		for (int i = 0; i < args.size(); i++) {
			ss << args[i]->string();

			if (i < args.size() - 1)
				ss << ", ";
		}
		ss << ')';

		return ss.str();
	}
	const void gen(IScope& scope, std::vector<unsigned char>& out) const override;
};
struct ASTArray : ASTExpr {
	const std::unique_ptr<const ASTExpr> arr;
	const std::unique_ptr<const ASTExpr> index;

	ASTArray(const unsigned int& line, std::unique_ptr<const ASTExpr>& arr, std::unique_ptr<const ASTExpr>& index) :
		ASTExpr(ASTType::A_ARRAY, line), arr(std::move(arr)), index(std::move(index)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << arr->string() << "[" << (index ? index->string() : "") << "]";

		return ss.str();
	}
};
struct ASTVar : ASTExpr {
	const std::unique_ptr<const ASTExpr> varType;
	const std::unique_ptr<const ASTExpr> name;

	ASTVar(const unsigned int& line, std::unique_ptr<const ASTExpr>& type, std::unique_ptr<const ASTExpr>& name) :
		ASTExpr(ASTType::A_VAR, line), varType(std::move(type)), name(std::move(name)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << varType->string() << ' ' << name->string();

		return ss.str();
	}
};
struct ASTAssign : ASTExpr {
	const std::unique_ptr<const ASTExpr> dest;
	const std::unique_ptr<const ASTExpr> value;

	ASTAssign(const unsigned int& line, std::unique_ptr<const ASTExpr>& dest, std::unique_ptr<const ASTExpr>& value) :
		ASTExpr(ASTType::A_ASSIGN, line), dest(std::move(dest)), value(std::move(value)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << dest->string() << " = " << value->string();

		return ss.str();
	}
	const void gen(IScope& scope, std::vector<unsigned char>& out) const override;
};

struct ASTDef : ASTExpr {
	const std::unique_ptr<const ASTExpr> expr;

	ASTDef(const unsigned int& line, std::unique_ptr<const ASTExpr>& expr) :
		ASTExpr(ASTType::A_DEF, line), expr(std::move(expr)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << "def " << expr->string();

		return ss.str();
	}
};
struct ASTFunc : ASTExpr {
	const std::unique_ptr<const ASTExpr> funcType;
	const std::unique_ptr<const ASTExpr> name;
	const std::vector<std::unique_ptr<const ASTExpr>> params;
	std::unique_ptr<const ASTExpr> expr;

	ASTFunc(const unsigned int& line, std::unique_ptr<const ASTExpr>& type, std::unique_ptr<const ASTExpr>& name, std::vector<std::unique_ptr<const ASTExpr>>& params, std::unique_ptr<const ASTExpr>& expr) :
		ASTExpr(ASTType::A_FUNC, line), funcType(std::move(type)), name(std::move(name)), params(std::move(params)), expr(std::move(expr)) {}
	const std::string string() const override {
		std::stringstream ss;
		ss << "func " << funcType->string() << " " << name->string() << "(";

		for (int i = 0; i < params.size(); i++) {
			ss << params[i]->string();

			if (i < params.size() - 1)
				ss << ", ";
		}
		ss << ") " << expr->string();

		return ss.str();
	}
	const void gen(IScope& scope, std::vector<unsigned char>& out) const override;
};
#endif