#include "parser.h"
#include <vector>
#include <memory>
#include "lexing/token.h"
#include "exception.h"
#include "common.h"

std::vector<std::unique_ptr<const Token>>* tokens;

inline const Token& token() {
	return *tokens->at(ptr);
}

inline const Token& next() {
	return *tokens->at(++ptr);
}

std::unique_ptr<const ASTExpr> parseBin();
std::unique_ptr<const ASTExpr> parseExpr();


std::unique_ptr<const ASTExpr> parseIdent() {
	auto ident = std::make_unique<ASTIdent>(token().line, static_cast<const SToken&>(token()).value);
	next();

	return ident;
}
std::unique_ptr<const ASTExpr> parseType() {
	auto left = parseIdent();

	while (token().type == TokenType::T_PERIOD) {
		next();
		auto right = parseType();

		left = std::make_unique<ASTAccess>(left->line, left, right);
	}

	if (token().type == TokenType::T_LBRACK) {
		next();
		if (token().type != TokenType::T_RBRACK) throw InvalidTokenException(token());
		std::unique_ptr<const ASTExpr> null{};
		left = std::make_unique<ASTArray>(left->line, left, null);
	}

	return left;
}
std::unique_ptr<const ASTExpr> parseCall() {
	auto callee = parseIdent();

	if (token().type == TokenType::T_LPAREN) {
		next();

		std::vector<std::unique_ptr<const ASTExpr>> args;
		while (token().type != TokenType::T_RPAREN) {
			args.push_back(parseBin());

			if (token().type == TokenType::T_COMMA)
				next();
			else if (token().type == TokenType::T_RPAREN)
				break;
			else throw InvalidTokenException(token());
		}
		next();

		callee = std::make_unique<ASTCall>(callee->line, callee, args);
	}

	return callee;
}
std::unique_ptr<const ASTExpr> parseArray() {
	auto ident = parseCall();

	if (token().type == TokenType::T_LBRACK) {
		next();
		std::unique_ptr<const ASTExpr> index{};

		if (token().type != TokenType::T_RBRACK) {
			index = parseBin();

			if (token().type != TokenType::T_RBRACK) throw InvalidTokenException(token());
		}
		next();

		ident = std::make_unique<ASTArray>(ident->line, ident, index);
	}

	return ident;
}
std::unique_ptr<const ASTExpr> parseAccess() {
	auto left = parseArray();

	if (token().type == TokenType::T_PERIOD) {
		next();
		auto right = parseAccess();

		left = std::make_unique<const ASTAccess>(left->line, left, right);
	}

	return left;
}
std::unique_ptr<const ASTExpr> parseVar() {
	auto first = parseAccess();

	if (token().type == TokenType::T_IDENTIFIER) {
		auto second = parseIdent();

		first = std::make_unique<ASTVar>(first->line, first, second);
	}

	return first;
}
std::unique_ptr<const ASTExpr> parseFac() {
	std::unique_ptr<const ASTExpr> fac;

	if (token().type == TokenType::T_INT)
		fac = std::make_unique<const ASTInt>(token().line, static_cast<const NToken&>(token()).value);
	else if (token().type == TokenType::T_FLOAT)
		fac = std::make_unique<const ASTFloat>(token().line, static_cast<const NToken&>(token()).value);
	else if (token().type == TokenType::T_CHAR)
		fac = std::make_unique<const ASTFloat>(token().line, static_cast<const NToken&>(token()).value);
	else if (token().type == TokenType::T_BOOL)
		fac = std::make_unique<const ASTFloat>(token().line, static_cast<const CToken&>(token()).value);
	else if (token().type == TokenType::T_STRING)
		fac = std::make_unique<const ASTString>(token().line, static_cast<const SToken&>(token()).value);
	else if (token().type == TokenType::T_PLUS || token().type == TokenType::T_MINUS) {
		const unsigned int& line = token().line;
		UnaryOP op = UnaryOP::U_PLUS;
		if (token().type == TokenType::T_MINUS)
			op = UnaryOP::U_MINUS;

		fac = parseFac();
		fac = std::make_unique<ASTUnary>(line, op, fac);
	}
	else if (token().type == TokenType::T_LPAREN) {
		next();
		fac = parseExpr();

		if (token().type != TokenType::T_RPAREN) throw InvalidTokenException(token());
	}
	else if (token().type == TokenType::T_IDENTIFIER)
		return parseVar();
	else throw InvalidTokenException(token());
	next();

	return fac;
}
std::unique_ptr<const ASTExpr> parseMul() {
	auto left = parseFac();

	if (token().type == TokenType::T_MUL) {
		next();

		auto right = parseMul();
		left = std::make_unique<const ASTBinary>(left->line, left, '*', right);
	}

	return left;
}
std::unique_ptr<const ASTExpr> parseDiv() {
	auto left = parseMul();

	if (token().type == TokenType::T_DIV) {
		next();

		auto right = parseDiv();
		left = std::make_unique<const ASTBinary>(left->line, left, '/', right);
	}

	return left;
}
std::unique_ptr<const ASTExpr> parseBin() {
	auto left = parseDiv();

	if (token().type == TokenType::T_PLUS || token().type == TokenType::T_MINUS) {
		const unsigned char op = static_cast<const CToken&>(token()).value;
		next();

		auto right = parseBin();

		left = std::make_unique<const ASTBinary>(left->line, left, op, right);
	}

	return left;
}

std::unique_ptr<const ASTExpr> parseExpr() {
	if (token().type == TokenType::T_LBRACE) {
		const unsigned int& line = token().line;
		std::vector<std::unique_ptr<const ASTExpr>> exprs;
		next();

		while (token().type != TokenType::T_RBRACE) {
			auto expr = parseExpr();
			exprs.push_back(std::move(expr));
		}
		next();

		return std::make_unique<ASTBlock>(line, exprs);
	}
	else return parseBin();
}
std::unique_ptr<const ASTExpr> parseField(unsigned const int& line) {
	next();
	if (token().type != TokenType::T_IDENTIFIER) throw InvalidTokenException(token());
	auto type = parseType();

	if (token().type != TokenType::T_IDENTIFIER) throw InvalidTokenException(token());
	auto name = parseIdent();

	return std::make_unique<ASTVar>(line, type, name);
}
std::unique_ptr<const ASTExpr> parseFunc(unsigned const int& line) {
	next();
	if (token().type != TokenType::T_IDENTIFIER) throw InvalidTokenException(token());
	auto type = parseType();

	if (token().type != TokenType::T_IDENTIFIER) throw InvalidTokenException(token());
	auto name = parseIdent();

	if (token().type != TokenType::T_LPAREN) throw InvalidTokenException(token());
	next();

	std::vector<std::unique_ptr<const ASTExpr>> params;
	if (token().type != TokenType::T_RPAREN) {
		while (true) {
			auto param = parseVar();
			params.push_back(std::move(param));

			if (token().type == TokenType::T_COMMA)
				continue;
			else if (token().type == TokenType::T_RPAREN)
				break;
			else throw InvalidTokenException(token());
		}
	}
	next();
	auto expr = parseExpr();

	return std::make_unique<ASTFunc>(line, type, name, params, expr);
}
std::unique_ptr<const ASTExpr> parseDef(unsigned const int& line) {
	next();

	if (token().type == TokenType::T_FUNC)
		return parseFunc(line);
	else if (token().type == TokenType::T_VAR)
		return parseField(line);
	else throw InvalidTokenException(token());
}
std::unique_ptr<const ASTExpr> parseClass() {
	if (token().type == TokenType::T_DEF)
		return parseDef(token().line);
	else throw InvalidTokenException(token());
	return parseExpr();
}

std::vector<std::unique_ptr<const ASTExpr>> parse(std::vector<std::unique_ptr<const Token>>* _tokens) {
	std::vector<std::unique_ptr<const ASTExpr>> exprs;
	tokens = _tokens;
	ptr = 0;

	for (int i = 0; i < tokens->size(); i++) {
		if (token().type == TokenType::T_EOF) break;
		std::unique_ptr<const ASTExpr> expr = parseClass();
		std::cout << expr->string() << '\n';
		exprs.push_back(std::move(expr));
	}

	return exprs;
}