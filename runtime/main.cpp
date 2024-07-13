#include <iostream>

#include <typeindex>
#include "opcodes.h"
#include "vm.h"
#include "xi.h"

#define MAJOR_VER			0
#define MINOR_VER			1

int main() {
    unsigned char bytecode[] = {
		1,
		0,
        OPCODE_NEWSTR, 'H','e','l','l','o',' ','w','o','r','l','d',0,
		OPCODE_NONE,
		OPCODE_RETURN
    };
    run_bytecode(bytecode, 0);

    return 0;
}