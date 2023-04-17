#include "exception.h"
#include <sstream>
#include <string>
#include <memory>
#include <lexing/token.h>

const char* BaseException::what() const throw() {
	return message.c_str();
}

const unsigned int BaseException::line() {
	return _line;
}

UnexpectedCharacterException::UnexpectedCharacterException(const char& _char, const unsigned int& line) : BaseException(line) {
	std::stringstream ss;
	ss << "Unexpected character: " << _char;
	message = ss.str();
}

ExpectedCharacterException::ExpectedCharacterException(const char& got, const char& exp, const unsigned int& line) : BaseException(line) {
	std::stringstream ss;
	ss << '\'' << got << "' expected " << exp;
	message = ss.str();
}


InvalidTokenException::InvalidTokenException(const Token& token) : BaseException(token.line) {
	std::stringstream ss;
	ss << "Invalid token: ";

	if (token.baseType == BaseType::BT_STRING)
		ss << static_cast<const SToken&>(token).value;
	else if (token.baseType == BaseType::BT_CHAR)
		ss << static_cast<const CToken&>(token).value;
	else if (token.baseType == BaseType::BT_NUM)
		ss << static_cast<const NToken&>(token).value;
	message = ss.str();
}

ExpectedException::ExpectedException(const char& _char, const unsigned int& line) : BaseException(line) {
	std::stringstream ss;
	ss << "Expected: " << _char;

	message = ss.str();
}

ExpectedAssignException::ExpectedAssignException(const unsigned int& line) : BaseException(line) {
	std::stringstream ss;
	ss << "Expected variable assignation";

	message = ss.str();
}

UnknownTypeException::UnknownTypeException(const std::string& type) : BaseException(0) {
	std::stringstream ss;
	ss << "Unknown type: " << type;

	message = ss.str();
}

UndefinedClassException::UndefinedClassException(const std::string& name) : BaseException(0) {
	std::stringstream ss;
	ss << "Reference to undefined class: " << name;

	message = ss.str();
}

GenericCountUnmatchException::GenericCountUnmatchException(const unsigned int& given, const unsigned int& exp, const std::string& className) : BaseException(0) {
	std::stringstream ss;
	ss << "The generic type count of " << given << " does not match with the expected count of " << exp << " for " << className;
	
	message = ss.str();
}
