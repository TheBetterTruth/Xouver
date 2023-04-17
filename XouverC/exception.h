#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <string>
#include <memory>
#include <lexing/token.h>

class BaseException : public std::exception {
private:
	const unsigned int _line;
	std::string message;
public:
	BaseException(const unsigned int& line) : _line(line) {}
	virtual const char* what() const throw();
	const unsigned int line();
	friend class UnexpectedCharacterException;
	friend class ExpectedCharacterException;
	friend class InvalidTokenException;
	friend class ExpectedException;
	friend class ExpectedAssignException;
	friend class UnknownTypeException;
	friend class UndefinedClassException;
	friend class GenericCountUnmatchException;
};

class UnexpectedCharacterException : public BaseException {
public:
	UnexpectedCharacterException(const char& _char, const unsigned int& line);
};

class ExpectedCharacterException : public BaseException {
public:
	ExpectedCharacterException(const char& got, const char& exp, const unsigned int& line);
};

class InvalidTokenException : public BaseException {
public:
	InvalidTokenException(const Token& token);
};

class ExpectedException : public BaseException {
public:
	ExpectedException(const char& _car, const unsigned int& line);
};
class ExpectedAssignException : public BaseException {
public:
	ExpectedAssignException(const unsigned int& line);
};
class UnknownTypeException : public BaseException {
public:
	UnknownTypeException(const std::string& type);
};
class UndefinedClassException : public BaseException {
public:
	UndefinedClassException(const std::string& name);
};
class GenericCountUnmatchException : public BaseException {
public:
	GenericCountUnmatchException(const unsigned int& given, const unsigned int& exp, const std::string& className);
};

#endif