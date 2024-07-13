#ifndef VM_H
#define VM_H

#include "memory.h"

typedef enum vm_version {
	V_0_1
};

bool valid_bytecode(unsigned char* _Bytecode);
vm_version bytecode_version(unsigned char* _Bytecode);
bool version_match(vm_version _Target);
size_t get_entry_address(unsigned char* _Bytecode);
void run_bytecode(unsigned char* _Bytecode, size_t _EntryAddress);

#endif