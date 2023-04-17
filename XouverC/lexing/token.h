#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {
	T_NONE,
	T_EOF,
	T_EOL,
	T_IDENTIFIER,
	T_INT,
	T_FLOAT,
	T_CHAR,
	T_STRING,
	T_BOOL,
	T_COMMA,
	T_PERIOD,
	T_LPAREN,
	T_RPAREN,
	T_LBRACK,
	T_RBRACK,
	T_LBRACE,
	T_RBRACE,
	T_PLUS,
	T_MINUS,
	T_MUL,
	T_DIV,
	T_COLON,
	T_COMP,
	T_LOGIC,
	T_ASSIGN,
	T_DEF,
	T_MOD,
	T_VAR,
	T_FUNC,
	T_IF,
	T_ELSE,
	T_WHILE,
	T_FOR,
	T_IN,
	T_IMPORT,
	T_IMPLEMENT
};

enum BaseType {
	BT_STRING,
	BT_NUM,
	BT_CHAR
};

struct Token {
	const TokenType type;
	const unsigned int line;
	const BaseType baseType;

	Token(const TokenType type, const unsigned int line, const BaseType baseType) :
		type(type), line(line), baseType(baseType) {}
};

template <typename T>
struct BaseToken : public Token {
	const T value;

	BaseToken(const TokenType type, const unsigned int line, const T value, const BaseType baseType) :
		Token(type, line, baseType), value(value) {};
};
struct SToken : public BaseToken<std::string> {
	SToken(const TokenType type, const unsigned int line, const std::string value) :
		BaseToken(type, line, value, BaseType::BT_STRING) {}
};
struct NToken : public BaseToken<float> {
	NToken(const TokenType type, const unsigned int line, const float value) :
		BaseToken(type, line, value, BaseType::BT_NUM) {}
};
struct CToken : public BaseToken<char> {
	CToken(const TokenType type, const unsigned int line, const char value) :
		BaseToken(type, line, value, BaseType::BT_CHAR) {}
};

#endif