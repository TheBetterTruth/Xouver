#include <stack>
#include <vector>
#include <memory>
#include <stdio.h>
#include <map>
#include <cstring>
#include <string>
#include "memory.h"

#include "vm.h"
#include "opcodes.h"

#define VM_VERSION			vm_version::V_0_1
#define VERSION_MAJOR		0
#define VERSION_MINOR		1

std::stack<object_ptr> stack{};
std::stack<std::vector<object_ptr>> scope_stack{};
std::stack<unsigned char*> inst_ptr_stack{};

bool valid_bytecode(unsigned char* _Bytecode) {
	return *((size_t*)_Bytecode) == 0x93888;
}
vm_version bytecode_version(unsigned char* _Bytecode) {
	_Bytecode += sizeof(size_t);

	unsigned char major = *(_Bytecode++);
	unsigned char minor = *_Bytecode;

	switch (major) {
		case 0: 
			switch (minor) {
				case 1:
					return V_0_1;
			}
	}
}

bool version_match(vm_version _Target) {
	return _Target == VM_VERSION;
}

size_t get_entry_address(unsigned char* _Bytecode) {
	return *((size_t*)_Bytecode + sizeof(size_t) + 2);
}

#pragma region Object_Wrapper_Funcs
template<typename T>
object_ptr make_obj(T& _Value) {
	object_ptr ptr = alloc_obj();

	T* data = new T();
	*data = _Value;
	ptr->data = data;

	return ptr;
}
object_ptr new_obj(size_t _Size) {
	object_ptr ptr = alloc_obj();
	object_ptr* data = new object_ptr[_Size];

	ptr->data = data;

	return ptr;

}

object_ptr new_string(const char* _Str) {
	object_ptr ptr = alloc_obj();

	ptr->data = std::string(_Str);

	return ptr;
}
#pragma endregion


#pragma region Stack_Push_Wrappers
void stack_push(object_ptr&& _Ptr) {
	stack.push(_Ptr);
}
void stack_push(object_ptr& _Ptr) {
	stack.push(_Ptr);
}
template <typename T>
void push_stack(T& _Value) {
	stack_push(make_obj(_Value));
}

void push_char(_xchar&& _Char) {
	push_stack(_Char);
}

void push_short(_xshort&& _Short) {
	push_stack(_Short);
}

void push_int(_xint&& _Int) {
	push_stack(_Int);
}

void push_float(_xfloat&& _Float) {
	push_stack(_Float);
}

void push_long(_xlong&& _Long) {
	push_stack(_Long);
}

void push_double(_xdouble&& _Double) {
	push_stack(_Double);
}

void push_string(const char* _Str) {
	stack_push(new_string(_Str));
}
#pragma endregion

#pragma region Stack_Pop_Wrappers
object_ptr stack_pop() {
	object_ptr top = stack.top();
	stack.pop();
	return top;
}

_xchar pop_char() {
	object_ptr ptr = stack_pop();
	return *std::get<xchar>(ptr->data);
}
_xshort pop_short() {
	object_ptr ptr = stack_pop();
	return *std::get<xshort>(ptr->data);
}
_xint pop_int() {
	object_ptr ptr = stack_pop();
	return *std::get<xint>(ptr->data);
}
_xfloat pop_float() {
	object_ptr ptr = stack_pop();
	return *std::get<xfloat>(ptr->data);
}
_xlong pop_long() {
	object_ptr ptr = stack_pop();
	return *std::get<xlong>(ptr->data);
}
_xdouble pop_double() {
	object_ptr ptr = stack_pop();
	return *std::get<xdouble>(ptr->data);
}

std::string pop_string() {
	object_ptr ptr = stack_pop();
	std::string str = std::get<std::string>(ptr->data);

	return str;
}

#pragma endregion

#pragma region Scope_Functions
std::vector<object_ptr>& scope() {
	return scope_stack.top();
}

void scope_store(uint8_t _Index, object_ptr&& _Ptr) {
	scope()[_Index] = _Ptr;
}

void scope_new(unsigned char*& _Ptr) {
	uint8_t scope_size = *((uint8_t*)_Ptr);
	_Ptr += sizeof(uint8_t);
	scope_stack.push(std::vector<object_ptr>(scope_size));
				

	uint8_t args_count = *((uint8_t*)_Ptr);
	_Ptr += sizeof(uint8_t);
	for (int i = args_count-1; i >= 0; i--) {
		scope_store(i, stack_pop());
	}
}

object_ptr scope_load(uint8_t _Index) {
	return scope()[_Index];
}
#pragma endregion

_xint arr_len(object_ptr& _Arr) {
	return sizeof(_Arr->data) / sizeof(object_ptr);
}

_xint arr_len(object_ptr&& _Arr) {
	return arr_len(_Arr);
}

void run_bytecode(unsigned char* _Bytecode, size_t _EntryAddress) {
	unsigned char* inst = _Bytecode + _EntryAddress;

	scope_new(inst);

	while (true) {
		switch (*inst) {

#pragma region Alloc_Ops
	#pragma region Int_Allocs
			case OPCODE_I0:
				push_int(0);
				break;
			case OPCODE_I1:
				push_int(1);
				break;
			case OPCODE_I2:
				push_int(2);
				break;
			case OPCODE_I3:
				push_int(3);
				break;
			case OPCODE_ICONST:
				push_stack(*((xint)inst));
				inst += sizeof(_xint)+1;
				break;
	#pragma endregion

	#pragma region Float_Allocs
			case OPCODE_F0:
				push_float(0.0f);
				break;
			case OPCODE_F1:
				push_float(1.0f);
				break;
			case OPCODE_F2:
				push_float(2.0f);
				break;
			case OPCODE_F3:
				push_float(3.0f);
				break;
			case OPCODE_FCONST: {
				push_stack(*((xfloat)inst));
				inst += sizeof(_xfloat)+1;
				break;
			}
	#pragma endregion

	#pragma region Bool_Allocs
			case OPCODE_C0:
				push_char('\0');
				break;
			case OPCODE_C1:
				push_char('\1');
				break;
	#pragma endregion

	#pragma region Other_Allocs
			// TODO: Implement. All of it.
			case OPCODE_CCONST: {
				push_stack(*((xchar)inst));
				inst += sizeof(_xchar)+1;
				break;
			}
			case OPCODE_SCONST: {
				push_stack(*((xshort)inst));
				inst += sizeof(_xshort)+1;
				break;
			}
			case OPCODE_LCONST: {
				push_stack(*((xlong)inst));
				inst += sizeof(_xlong)+1;
				break;
			}
			case OPCODE_DCONST: {
				push_stack(*((xdouble)inst));
				inst += sizeof(_xdouble)+1;
				break;
			}
			case OPCODE_STRCONST: {
				const char* str = (char*) ++inst;
				while (*inst != 0)
					inst++;
				
				push_string(str);
				break;
			}
	#pragma endregion
#pragma endregion

#pragma region Conv_Ops
	#pragma region Int_Convs
			case OPCODE_ITOC:
				push_char(pop_int());
				break;
			case OPCODE_ITOS:
				push_short(pop_int());
				break;
			case OPCODE_ITOF:
				push_float(pop_int());
				break;
			case OPCODE_ITOL:
				push_long(pop_int());
				break;
			case OPCODE_ITOD:
				push_double(pop_int());
				break;
	#pragma endregion

	#pragma region Float_Convs
			case OPCODE_FTOC:
				push_char(pop_float());
				break;
			case OPCODE_FTOS:
				push_short(pop_float());
				break;
			case OPCODE_FTOI:
				push_int(pop_float());
				break;
			case OPCODE_FTOL:
				push_long(pop_float());
				break;
			case OPCODE_FTOD:
				push_double(pop_float());
				break;
	#pragma endregion

	#pragma region Char_Convs
			case OPCODE_CTOS:
				push_short(pop_char());
				break;
			case OPCODE_CTOI:
				push_int(pop_char());
				break;
			case OPCODE_CTOF:
				push_float(pop_char());
				break;
			case OPCODE_CTOL:
				push_long(pop_char());
				break;
			case OPCODE_CTOD:
				push_double(pop_char());
				break;
	#pragma endregion

	#pragma region Short_Convs
			case OPCODE_STOC:
				push_char(pop_short());
				break;
			case OPCODE_STOI:
				push_int(pop_short());
				break;
			case OPCODE_STOF:
				push_float(pop_short());
				break;
			case OPCODE_STOL:
				push_long(pop_short());
				break;
			case OPCODE_STOD:
				push_double(pop_short());
				break;
	#pragma endregion

	#pragma region Long_Convs
			case OPCODE_LTOC:
				push_char(pop_long());
				break;
			case OPCODE_LTOS:
				push_short(pop_long());
				break;
			case OPCODE_LTOI:
				push_int(pop_long());
				break;
			case OPCODE_LTOF:
				push_float(pop_long());
				break;
			case OPCODE_LTOD:
				push_double(pop_long());
				break;
	#pragma endregion

	#pragma region Double_Convs
			case OPCODE_DTOC:
				push_char(pop_double());
				break;
			case OPCODE_DTOS:
				push_short(pop_double());
				break;
			case OPCODE_DTOI:
				push_int(pop_double());
				break;
			case OPCODE_DTOF:
				push_float(pop_double());
				break;
			case OPCODE_DTOL:
				push_long(pop_double());
				break;
	#pragma endregion
#pragma endregion

#pragma region Int_Ops
			case OPCODE_ADDI: {
				_xint rval = pop_int();
				_xint lval = pop_int();

				push_int(lval + rval);
				break;
			}
			case OPCODE_SUBI: {
				_xint rval = pop_int();
				_xint lval = pop_int();

				push_int(lval - rval);
				break;
			}
			case OPCODE_MULI: {
				_xint rval = pop_int();
				_xint lval = pop_int();

				push_int(lval * rval);
				break;
			}
			case OPCODE_DIVI: {
				_xint rval = pop_int();
				_xint lval = pop_int();

				push_int(lval / rval);
				break;
			}
			case OPCODE_CMPI: {
				_xint rval = pop_int();
				_xint lval = pop_int();

				if (lval < rval)
					push_char(2);
				else if (lval > rval)
					push_char(3);
				else
					push_char(1);
				break;
			}
#pragma endregion

#pragma region Float_Ops
			case OPCODE_ADDF: {
				_xfloat rval = pop_float();
				_xfloat lval = pop_float();

				push_float(lval + rval);
				break;
			}
			case OPCODE_SUBF: {
				_xfloat rval = pop_float();
				_xfloat lval = pop_float();

				push_float(lval - rval);
				break;
			}
			case OPCODE_MULF: {
				_xfloat rval = pop_float();
				_xfloat lval = pop_float();

				push_float(lval * rval);
				break;
			}
			case OPCODE_DIVF: {
				_xfloat rval = pop_float();
				_xfloat lval = pop_float();

				push_float(lval / rval);
				break;
			}
			case OPCODE_CMPF: {
				_xfloat rval = pop_float();
				_xfloat lval = pop_float();

				if (lval < rval)
					push_char(2);
				else if (lval > rval)
					push_char(3);
				else
					push_char(1);
				break;
			}
#pragma endregion

#pragma region Short_Ops
			case OPCODE_ADDS: {
				_xshort rval = pop_short();
				_xshort lval = pop_short();

				push_short(lval + rval);
				break;
			}
			case OPCODE_SUBS: {
				_xshort rval = pop_short();
				_xshort lval = pop_short();

				push_short(lval - rval);
				break;
			}
			case OPCODE_MULS: {
				_xshort rval = pop_short();
				_xshort lval = pop_short();

				push_short(lval * rval);
				break;
			}
			case OPCODE_DIVS: {
				_xshort rval = pop_short();
				_xshort lval = pop_short();

				push_short(lval / rval);
				break;
			}
			case OPCODE_CMPS: {
				_xshort rval = pop_short();
				_xshort lval = pop_short();

				if (lval < rval)
					push_int(2);
				else if (lval > rval)
					push_int(3);
				else
					push_int(1);
				break;
			}	
#pragma endregion

#pragma region Long_Ops
			case OPCODE_ADDL: {
				_xlong rval = pop_long();
				_xlong lval = pop_long();

				push_long(lval + rval);
				break;
			}
			case OPCODE_SUBL: {
				_xlong rval = pop_long();
				_xlong lval = pop_long();

				push_long(lval - rval);
				break;
			}
			case OPCODE_MULL: {
				_xlong rval = pop_long();
				_xlong lval = pop_long();

				push_long(lval * rval);
				break;
			}
			case OPCODE_DIVL: {
				_xlong rval = pop_long();
				_xlong lval = pop_long();

				push_long(lval / rval);
				break;
			}
			case OPCODE_CMPL: {
				_xlong rval = pop_long();
				_xlong lval = pop_long();

				if (lval < rval)
					push_int(2);
				else if (lval > rval)
					push_int(3);
				else
					push_int(1);
				break;
			}
#pragma endregion

#pragma region Double_Ops
			case OPCODE_ADDD: {
				_xdouble rval = pop_double();
				_xdouble lval = pop_double();

				push_double(lval + rval);
				break;
			}
			case OPCODE_SUBD: {
				_xdouble rval = pop_double();
				_xdouble lval = pop_double();

				push_double(lval - rval);
				break;
			}
			case OPCODE_MULD: {
				_xdouble rval = pop_double();
				_xdouble lval = pop_double();

				push_double(lval * rval);
				break;
			}
			case OPCODE_DIVD: {
				_xdouble rval = pop_double();
				_xdouble lval = pop_double();

				push_double(lval / rval);
				break;
			}
			case OPCODE_CMPD: {
				_xdouble rval = pop_double();
				_xdouble lval = pop_double();

				if (lval < rval)
					push_int(2);
				else if (lval > rval)
					push_int(3);
				else
					push_int(1);
				break;
			}
#pragma endregion

#pragma region Boolean_Ops
			case OPCODE_NEG:
				push_char(pop_char() == 0 ? 1 : 0);
				break;
			case OPCODE_AND:
				push_char(pop_char() == 1 && pop_char() == 1 ? 1 : 0);
				break;
			case OPCODE_OR:
				push_char(pop_char() == 1 || pop_char() == 1 ? 1 : 0);
				break;

			/* 
				Comparision values:
					1 ->	Equals
					2 ->	Less
					3 ->	Greater
			*/

			case OPCODE_EQ:
				push_char(pop_char() == 1 ? 1 : 0);
				break;
			case OPCODE_LT:
				push_char(pop_char() == 2 ? 1 : 0);
				break;
			case OPCODE_GT:
				push_char(pop_char() == 3 ? 1 : 0);
				break;
			case OPCODE_LQ: {
				_xchar c = pop_char();

				push_char(c == 1 || c == 2 ? 1 : 0);
				break;
			}
			case OPCODE_GQ: {
				_xchar c = pop_char();
				
				push_char(c == 1 || c == 3 ? 1 : 0);
				break;
			}
#pragma endregion

#pragma region Jump_Ops
			case OPCODE_JMP:
				inst += *((int32_t*)inst);
				continue;
			case OPCODE_JIF: {
				_xchar val = pop_char();

				if (val == 1)
					inst += *((int32_t*)inst);
				else
					inst += sizeof(int32_t)+1;
				
				continue;
			}
#pragma endregion

#pragma region String_Ops
			// TODO: Implement.
#pragma endregion

#pragma region Storage_Ops
			case OPCODE_LSTORE:
				scope_store(*((uint8_t*)++inst), stack_pop());
				inst += sizeof(uint8_t);
				continue;
			case OPCODE_LLOAD:
				stack_push(scope_load(*((uint8_t*)++inst)));
				inst += sizeof(uint8_t);
				continue;

			case OPCODE_OSTORE: {
				object_ptr value = stack_pop();
				std::get<object_ptr*>(stack_pop()->data)[*((uint8_t*)++inst)] = value;
				inst += sizeof(uint8_t);
				continue;
			}
			case OPCODE_OLOAD: {
				stack_push(std::get<object_ptr*>(stack_pop()->data)[*((uint8_t*)++inst)]);
				inst += sizeof(uint8_t);
				continue;
			}
			case OPCODE_ASTORE: {
				object_ptr value = stack_pop();
				object_ptr arr = stack_pop();
				_xint index = pop_int();

				std::get<object_ptr*>(arr->data)[index] = value;
				break;
			}
			case OPCODE_ALOAD: {
				object_ptr arr = stack_pop();
				_xint index = pop_int();

				stack_push(std::get<object_ptr*>(arr->data)[index]);
				break;
			}
#pragma endregion

#pragma region Function_Ops
			case OPCODE_CALL: {
				inst++;
				size_t address = *((size_t*)inst);
				inst_ptr_stack.push(inst + sizeof(size_t));
				inst = _Bytecode + address;

				scope_new(inst);
				continue;
			}
			case OPCODE_RETURN:
				if (!inst_ptr_stack.empty()) {
					inst = inst_ptr_stack.top();
					inst_ptr_stack.pop();
				}
				else
					return;
				continue;
#pragma endregion

#pragma region Array_Ops
			case OPCODE_ALEN: {
				push_int(arr_len(stack_pop()));
				break;
			}
			case OPCODE_AFILL: {
				object_ptr arr = stack_pop();
				_xint len = arr_len(arr);

				for (_xint i = len-1; i >= 0; i--) {
					std::get<object_ptr*>(arr->data)[i] = stack_pop();
				}
				break;
			}
#pragma endregion

#pragma region New_Ops
			case OPCODE_NEWO: {
				uint8_t fields_count = *((uint8_t*)++inst);
				inst += sizeof(uint8_t);

				stack_push(new_obj(fields_count));
				continue;
			}
			case OPCODE_NEWSTR: {
				push_string("");
				break;
			}
			case OPCODE_NEWARR: {
				_xint len = pop_int();

				stack_push(new_obj(len));
				break;
			}
#pragma endregion

#pragma region Stack_Ops
			case OPCODE_POP: {
				stack_pop();
				break;
			}
			case OPCODE_DUP: {
				object_ptr ptr = stack.top();
				stack_push(ptr);
				break;
			}
#pragma endregion

			case OPCODE_NONE: {
				std::string str = pop_string();

				printf("%s", str.c_str());
				break;
			}
		}
		inst++;
	}
}