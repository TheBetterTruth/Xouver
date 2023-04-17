#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <memory>

extern std::vector<std::string> types;

struct Type {
	const std::string name;
	Type(const std::string& name) : name(name) {}
};

extern const Type null;

#endif