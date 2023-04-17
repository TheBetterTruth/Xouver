#include <string>
#include <vector>
#include <memory>
#include <istream>
#include <common.h>
#include <exception.h>
#include "token.h"

const std::string DIGITS = "1234567890";
const std::string LETTERS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

char current;
std::string lineStr;
unsigned int line;
bool inComment;

char advance();
std::unique_ptr<const Token> makeNumber();
std::unique_ptr<const Token> makeIdentifier();
std::unique_ptr<const Token> makeString();
std::unique_ptr<const Token> makeChar();

std::vector<std::unique_ptr<const Token>> lex(std::istream& is) {
	line = 1;
	std::vector<std::unique_ptr<const Token>> tokens;
	inComment = false;

	while (std::getline(is, lineStr)) {
		ptr = 0;
		current = lineStr[ptr];

		while (ptr < lineStr.size()) {
			if (!inComment) {
				if (match(DIGITS, current)) {
					tokens.push_back(makeNumber());
					continue;
				}
				else if (match(LETTERS, current) || current == '_') {
					tokens.push_back(makeIdentifier());
					continue;
				}
				else if (current == '"')
					tokens.push_back(makeString());
				else if (current == '\'')
					tokens.push_back(makeChar());
				else if (current == '+')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_PLUS, line, current));
				else if (current == '-')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_MINUS, line, current));
				else if (current == '*')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_MUL, line, current));
				else if (current == '/') {
					advance();
					if (current == '/')
						break;
					else if (current == '*') {
						inComment = true;
						break;
					}
					tokens.push_back(std::make_unique<CToken>(TokenType::T_DIV, line, '/'));
					continue;
				}
				else if (current == ':')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_COLON, line, current));
				else if (current == ',')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_COMMA, line, current));
				else if (current == '.')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_PERIOD, line, current));
				else if (current == '(')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_LPAREN, line, current));
				else if (current == ')')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_RPAREN, line, current));
				else if (current == '{')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_LBRACE, line, current));
				else if (current == '}')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_RBRACE, line, current));
				else if (current == '[')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_LBRACK, line, current));
				else if (current == ']')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_RBRACK, line, current));
				else if (current == '+')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_PLUS, line, current));
				else if (current == '=')
					if (advance() == '=')
						tokens.push_back(std::make_unique<SToken>(TokenType::T_COMP, line, "=="));
					else {
						tokens.push_back(std::make_unique<SToken>(TokenType::T_ASSIGN, line, "="));
						continue;
					}
				else if (current == '<')
					if (advance() == '=')
						tokens.push_back(std::make_unique<SToken>(TokenType::T_COMP, line, "<="));
					else {
						tokens.push_back(std::make_unique<SToken>(TokenType::T_COMP, line, "<"));
						continue;
					}
				else if (current == '>')
					if (advance() == '=')
						tokens.push_back(std::make_unique<SToken>(TokenType::T_COMP, line, ">="));
					else {
						tokens.push_back(std::make_unique<SToken>(TokenType::T_COMP, line, ">"));
						continue;
					}
				else if (current == '&')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_LOGIC, line, '&'));
				else if (current == '|')
					tokens.push_back(std::make_unique<CToken>(TokenType::T_LOGIC, line, '|'));
				else if (match(" \t\v", current)) {}
				advance();
			}
			else {
				if (current == '/')
					inComment = false;
			}
		}
		line++;
	}

	tokens.push_back(std::make_unique<CToken>(TokenType::T_EOF, line, 0));
	return tokens;
}

char advance() {
	if (++ptr >= lineStr.size())
		current = '\n';
	else
		current = lineStr[ptr];
	return current;
}

std::unique_ptr<const Token> makeNumber() {
	std::string numStr = "";
	bool hasDot = false;

	while (match(DIGITS, current) || current == '.') {
		if (current == '.') {
			if (hasDot) throw UnexpectedCharacterException(current, line);
			hasDot = true;
		}
		numStr += current;
		advance();
	}
	TokenType type = hasDot ? TokenType::T_FLOAT : TokenType::T_INT;
	return std::make_unique<NToken>(type, line, std::stof(numStr));
}
std::unique_ptr<const Token> makeIdentifier() {
	std::string ident;

	while (match(LETTERS, current) || match(DIGITS, current) || current == '_') {
		ident += current;
		advance();
	}

	TokenType type;
	if (ident == "true" || ident == "false")
		return std::make_unique<CToken>(TokenType::T_BOOL, line, ident == "true" ? 1 : 0);
	else if (ident == "def")
		type = TokenType::T_DEF;
	else if (ident == "var")
		type = TokenType::T_VAR;
	else if (ident == "func")
		type = TokenType::T_FUNC;
	else if (ident == "if")
		type = TokenType::T_IF;
	else if (ident == "else")
		type = TokenType::T_ELSE;
	else if (ident == "while")
		type = TokenType::T_WHILE;
	else if (ident == "for")
		type = TokenType::T_FOR;
	else if (ident == "in")
		type = TokenType::T_IN;
	else if (ident == "import")
		type = TokenType::T_IMPORT;
	else if (ident == "implement")
		type = TokenType::T_IMPLEMENT;
	else
		type = TokenType::T_IDENTIFIER;

	return std::make_unique<SToken>(type, line, ident);
}
std::unique_ptr<const Token> makeString() {
	std::string str = "";

	advance();
	while (current != '"') {
		if (current == '\n') throw UnexpectedCharacterException(current, line);

		str += current;
		advance();
	}

	return std::make_unique<SToken>(TokenType::T_STRING, line, str);
}
std::unique_ptr<const Token> makeChar() {
	std::string str;

	advance();
	while (current != '\0') {
		if (current == '\n') throw UnexpectedCharacterException(current, line);
		str += current;
		advance();
	}

	if (str.length() > 1) throw ExpectedCharacterException(current, '\'', line);
	else if (str.length() == 0) throw UnexpectedCharacterException(current, line);
	
	return std::make_unique<CToken>(TokenType::T_CHAR, line, str[0]);
}