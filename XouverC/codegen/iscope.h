#ifndef ISCOPE_H
#define ISCOPE_H

#include <vector>
#include <string>
#include "type.h"

class IScope {
public:
	virtual const bool has(const std::string& name) const { return false; }
	virtual const void get(const std::string& name, std::vector<unsigned char>& out) const {}
	virtual const void put(const Type type, const std::string& name, std::vector<unsigned char>& out) {}
	virtual const void getConst(const int& value, std::vector<unsigned char>& out) {}
	virtual const Type& typeOf(const std::string& name) const { return null; }
	virtual const unsigned int ptr() const { return 0; }
};

#endif