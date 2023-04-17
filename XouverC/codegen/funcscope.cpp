#include "funcscope.h"
#include "opcodes.h"

#include <string>
#include "iscope.h"
#include "type.h"
#include "common.h"
#include "scope.h"

FuncScope::FuncScope(IScope& classScope, const std::vector<Var>& params) : classScope(classScope), scope(*this), vars(params.size()) {
	scope = Scope(*this, params);
}

const void FuncScope::get(const std::string& name, std::vector<unsigned char>& out) const {
	if (scope.has(name))
		return scope.get(name, out);

	return classScope.get(name, out);
}
const void FuncScope::put(const Type type, const std::string& name, std::vector<unsigned char>& out) {
	scope.put(type, name, out);
}
const void FuncScope::getConst(const int& value, std::vector<unsigned char>& out) {
	return classScope.getConst(value, out);
}
const Type& FuncScope::typeOf(const std::string& name) const {
	if (scope.has(name))
		return scope.typeOf(name);

	return classScope.typeOf(name);
}
inline const unsigned int FuncScope::ptr() const {
	return vars;
}