#include "scope.h"
#include "opcodes.h"

#include <string>
#include <vector>
#include "iscope.h"
#include "type.h"
#include "common.h"

Scope::Scope(IScope& parent) : parent(parent) {}
Scope::Scope(IScope& parent, const std::vector<Var>& vars) : parent(parent) {
	for (auto& var : vars)
		this->vars.push_back(var);
}
const bool Scope::has(const std::string& name) const {
	for (auto& var : vars)
		if (var.name == name)
			return true;

	return false;
}

const void Scope::get(const std::string& name, std::vector<unsigned char>& out) const {
	for (auto& var : vars) {
		if (var.name == name) {
			out.push_back(OP_LOAD);
			write_bytes(&var.ptr, sizeof(var.ptr), out);
		}
	}
}
const void Scope::put(const Type type, const std::string& name, std::vector<unsigned char>& out) {
	Var var(type, name, ptr());
	vars.push_back(var);

	out.push_back(OP_STORE);
	write_bytes(&var.ptr, sizeof(var.ptr), out);
}
const void Scope::getConst(const int& value, std::vector<unsigned char>& out) {
	return parent.getConst(value, out);
}
const Type& Scope::typeOf(const std::string& name) const {
	for (auto var : vars)
		if (var.name == name)
			return var.type;

	return parent.typeOf(name);
}
inline const unsigned int Scope::ptr() const {
	return parent.ptr();
}