#ifndef CLASS_H
#define CLASS_H

#include "iscope.h"

#include <string>
#include <vector>
#include <map>
#include "type.h"
#include "var.h"
#include "function.h"
#include "parsing/ast.h"
#include "constant.h"

class Class : public IScope {
private:
	const std::string _name;
	const std::string _fullName;
	std::vector<Var> fields;
	std::vector<std::unique_ptr<Function>> functions;
	std::map<std::string, std::string> imports;
	std::vector<Constant> _constants;
public:
	const Class(const std::string& name, const std::string& fullname, const std::vector<std::unique_ptr<const ASTExpr>>& exprs);
	const void gen(std::vector<unsigned char>& out);
	const bool has(const std::string& name) const;
	const void get(const std::string& name, std::vector<unsigned char>& out) const;
	const void put(const Type type, const std::string& name, std::vector<unsigned char>& out);
	const void getConst(const int& value, std::vector<unsigned char>& out) override;
	const Type typeOf(const std::string& name) const override;
	const Type typeOf(const std::string& name, const std::vector<Type>& paramTypes) const override;
	const unsigned int ptr() const;
	const unsigned int funcCount() const;
	const unsigned int constCount() const;
	const Function& getFunc(unsigned int ptr) const;
	const std::vector<Constant>& constants() const;
	const void call(const std::string& name, const std::vector<Type>& paramTypes, std::vector<unsigned char>& out) const override;

	const std::string& name() const;
	const std::string& fullName() const;
};

extern std::vector<std::unique_ptr<Class>> classes;

#endif