#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <memory>

extern std::vector<std::string> types;

struct Type {
	const std::string name;
	Type(const std::string& name) : name(name) {}
	Type(const Type& src) : name(src.name) {}

	const Type operator=(const Type& src) { return Type(src.name); }
	const bool operator==(const Type t) const {
		if (t.name == name) return true;
		return false;
	}
};

extern const Type null;

#endif