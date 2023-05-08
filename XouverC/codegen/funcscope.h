#ifndef FUNCSCOPE_H
#define FUNCSCOPE_H

#include "iscope.h"

#include <string>
#include <vector>
#include "type.h"
#include "var.h"
#include "scope.h"

class FuncScope : public IScope {
private:
	IScope& classScope;
	Scope scope;
	unsigned int vars;
public:
	FuncScope(IScope& classScope, const std::vector<Var>& params);
	const void get(const std::string& name, std::vector<unsigned char>& out) const;
	const void put(const Type type, const std::string& name, std::vector<unsigned char>& out);
	const void getConst(const int& value, std::vector<unsigned char>& out) override;
	const Type typeOf(const std::string& name) const;
	const unsigned int ptr() const;
	const void advance() override;
	const void call(const std::string& name, const std::vector<Type>& paramTypes, std::vector<unsigned char>& out) const override;
};

#endif