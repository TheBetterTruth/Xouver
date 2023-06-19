#include "class.h"

#include <string>
#include <memory>
#include <vector>
#include "opcodes.h"
#include "type.h"
#include "parsing/ast.h"
#include "exception.h"
#include "funcscope.h"
#include "iscope.h"
#include "common.h"
#include "constant.h"

std::vector<std::unique_ptr<Class>> classes;

Class::Class(const std::string& name, const std::string& fullName, const std::vector<std::unique_ptr<const ASTExpr>>& exprs) : _name(name), _fullName(fullName) {
	for (auto& expr : exprs) {
		if (expr->type == ASTType::A_FUNC) {
			ASTFunc* func = (ASTFunc*) static_cast<const ASTFunc*>(expr.get());

			const Type type = type_from(func->funcType);
			if (func->name->type != ASTType::A_IDENT)
				throw std::exception();

			const std::string& name = static_cast<const ASTIdent*>(func->name.get())->value;
			std::string signature = type.name + ' ' + fullName + ':' + name + '(';

			std::vector<Var> params;
			for (int i = 0; i < func->params.size(); i++) {
				if (func->params[i]->type != ASTType::A_VAR)
					throw std::exception();
				
				const ASTVar* var = static_cast<const ASTVar*>(func->params[i].get());
				if (var->name->type != ASTType::A_IDENT)
					throw std::exception();
				const std::string& name = static_cast<const ASTIdent*>(var->name.get())->value;
				const Type type = type_from(var->varType);

				params.push_back(Var(type, name, params.size()));

				signature += type.name + ' ' + name;
				if (i < func->params.size() - 1)
					signature += ", ";
			}
			signature += ')';
			functions.push_back(std::make_unique<Function>(signature, type, name, params, func->expr));
		}
		else if (expr->type == ASTType::A_VAR) {
			const ASTVar* var = static_cast<const ASTVar*>(expr.get());

			if (var->name->type != ASTType::A_IDENT)
				throw std::exception();

			const std::string& name = static_cast<const ASTIdent*>(var->name.get())->value;
			fields.push_back(Var(type_from(var->varType), name, fields.size()));
		}
		else throw std::exception();
	}
}
const void Class::gen(std::vector<unsigned char>& out) {
	for (auto& func : functions) {
		FuncScope scope(*this, func->params);

		std::vector<unsigned char> code;
		func->ptr = out.size();
		func->expr->gen(scope, code);

		if (func->type.name == "void")
			code.push_back(OP_RET0);
		else
			code.push_back(OP_RET1);

		unsigned int scopeSize = scope.ptr();
		write_bytes(&scopeSize, sizeof(scopeSize), out);

		unsigned int argsCount = func->params.size();
		write_bytes(&argsCount, sizeof(argsCount), out);

		for (auto& c : code)
			out.push_back(c);
	}
}
const bool Class::has(const std::string& name) const {
	for (auto& field : fields)
		if (field.name == name)
			return true;

	return false;
}
const void Class::get(const std::string& name, std::vector<unsigned char>& out) const {
	for (auto& field : fields)
		if (field.name == name) {
			out.push_back(OP_OLOAD);
			write_bytes(&field.ptr, sizeof(&field.ptr), out);
		}

	throw std::exception();
}
const void Class::put(const Type type, const std::string& name, std::vector<unsigned char>& out) {
	throw std::exception();
}
const void Class::getConst(const int& value, std::vector<unsigned char>& out) {
	int p = -1;
	for (int i = 0; i < _constants.size(); i++) {
		if (_constants[i].type == ConstantType::C_INT && _constants[i].value.i == value)
			p = i;
	}

	if (p == -1) {
		_constants.push_back(Constant(value));
		p = _constants.size() - 1;
	}

	write_bytes(&p, sizeof(p), out);
}
const Type Class::typeOf(const std::string& name) const {
	for (auto& field : fields)
		if (field.name == name)
			return field.type;

	throw std::exception();
}
const Type Class::typeOf(const std::string& name, const std::vector<Type>& paramTypes) const {
	for (auto& func : functions) {
		if (func->name == name) {
			if (func->params.size() == paramTypes.size()) {
				bool keep = true;

				for (int i = 0; i < paramTypes.size(); i++) {
					if (paramTypes[i] != func->params[i].type) {
						keep = false;
						break;
					}
				}

				if (keep)
					return func->type;
			}
		}
	}

	throw std::exception();
}
const unsigned int Class::ptr() const {
	return fields.size();
}
const unsigned int Class::funcCount() const {
	return functions.size();
}
const unsigned int Class::constCount() const {
	return _constants.size();
}
const Function& Class::getFunc(unsigned int ptr) const {
	return *functions[ptr];
}
const std::vector<Constant>& Class::constants() const {
	return _constants;
}
const void Class::call(const std::string& name, const std::vector<Type>& paramTypes, std::vector<unsigned char>& out) const {
	for (auto& f : functions) {
		if (f->name == name && f->params.size() == paramTypes.size()) {
			bool match = true;

			for (int i = 0; i < paramTypes.size(); i++)
				if (paramTypes[i].name != f->params[i].type) {
					match = false;
					break;
				}
			if (!match) continue;

			out.push_back(OP_CALL);
			write_bytes(&f->ptr, sizeof(f->ptr), out);
		}
	}
}
const std::string& Class::name() const {
	return _name;
}

const std::string& Class::fullName() const {
	return _fullName;
}