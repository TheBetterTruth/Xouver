#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <memory>

extern std::vector<std::string> types;

struct Type {
	const std::string name;
	const bool array;
	Type(const std::string& name, const bool array) : name(name), array(array) {}
	Type(const Type& src) : name(src.name), array(src.array) {}

	const bool operator==(const Type t) const {
		return (t.name == name && t.array == array);
	}
};

extern const Type null;

#endif