#ifndef VAR_H
#define VAR_H

#include <string>
#include "type.h"

struct Var {
	const Type type;
	const std::string name;
	const unsigned int ptr;

	Var(const Type type, const std::string& name, const unsigned int ptr) :
		type(type), name(name), ptr(ptr) {}
	Var operator=(const Var& src) {
		return Var(src.type, src.name, src.ptr);
	}
};

#endif