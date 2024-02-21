#include "code_generator.h"

code_generator::code_generator() {

}

void code_generator::create_symbols(xpkg_symbol_table& _SymbolTable, class_node* _Node) {
	xpkg_symbol class_symbol;

	class_symbol.symbol_type = "class";
	class_symbol.type = "Xouver.Lang.Class";
	class_symbol.name = _Node->class_path + "." + _Node->class_name;
	class_symbol.base_type = "Xouver.Lang.Object";

	_SymbolTable.insert(class_symbol);

	for (auto& node : _Node->body) {
		xpkg_symbol symbol;

		if (node->type == VAR_DECL_NODE) {
			auto decl_node = dynamic_cast<var_decl_node*>(node.get());

			symbol.symbol_type = "field";
			symbol.name += class_symbol.base_type += "." + decl_node->name;
			symbol.type = decl_node->value_type;

			_SymbolTable.insert(symbol);
		}
		else if (node->type == FUNC_DECL_NODE) {
			auto decl_node = dynamic_cast<func_decl_node*>(node.get());

			symbol.symbol_type = "function";
			symbol.name = class_symbol.base_type += "." + decl_node->name;
			symbol.type = decl_node->return_type;

			for (auto& param : decl_node->params) {
				symbol.parameters.push_back(param.value_type);
			}
		}
		else {
			throw std::runtime_error("Unexpected node");
		}
	}
}