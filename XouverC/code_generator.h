#ifndef CODE_GENERATOR_H
#define CODE_GENERATOR_H

#include <stack>
#include <string>
#include <xpkg.h>
#include "nodes.h"

class code_generator {
private:
	typedef std::stack<std::string> type_stack;
public:
	code_generator();
	void create_symbols(xpkg_symbol_table& _SymbolTable, class_node* _Node);
	
	void visit_class(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, class_node* _Node);
	void visit_var_decl(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, var_decl_node* _Node);
	void visit_func_decl(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, func_decl_node* _Node);
	void visit_assign(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, assign_node* _Node);
	void visit_binop(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, binop_node* _Node);
	void visit_unop(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, unop_node* _Node);
	void visit_num(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, num_node* _Node);
	void visit_ident(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, ident_node* _Node);
	void visit_char(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, char_node* _Node);
	void visit_bool(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, bool_node* _Node);
	void visit_comp(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, condition_node* _Node);
	void visit_member(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, member_node* _Node);
	void visit_element(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, element_node* _Node);
	void visit_call(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, call_node* _Node);
	void visit_if(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, if_node* _Node);
	void visit_while(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, while_node* _Node);
	void visit_for(xpkg_symbol_table& _SymbolTable, xpkg_bytecode_stream& _BytecodeStream, type_stack& _TypeStack, for_node* _Node);









};

#endif