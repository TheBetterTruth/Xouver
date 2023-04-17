#ifndef SCOPE_H
#define SCOPE_H

#include "iscope.h"

#include <vector>
#include <string>
#include "var.h"

class Scope : public IScope {
private:
	IScope& parent;
	std::vector<Var> vars;

public:
	const Scope(IScope& parent);
	const Scope(IScope& parent, const std::vector<Var>& vars);
	const bool has(const std::string& name) const override;
	const void get(const std::string& name, std::vector<unsigned char>& out) const override;
	const void put(const Type type, const std::string& name, std::vector<unsigned char>& out) override;
	const void getConst(const int& value, std::vector<unsigned char>& out) override;
	const Type& typeOf(const std::string& name) const override;
	const unsigned int ptr() const override;
};

#endif