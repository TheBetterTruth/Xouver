#ifndef OPCODES_H
#define OPCODES_H

#define OPCODE_NONE			0x00		// Undefined operation

#pragma region Int_Alloc_Ops
#define OPCODE_I0			0x01		// Push int 0 to stack
#define OPCODE_I1			0x02		// Push int 1 to stack
#define OPCODE_I2			0x03		// Push int 2 to stack
#define OPCODE_I3			0x04		// Push int 3 to stack
#define OPCODE_ICONST		0x05		// Push int from constant pool to stack
#pragma endregion

#pragma region Float_Alloc_Ops
#define OPCODE_F0			0x06		// Push float 0 to stack
#define OPCODE_F1			0x07		// Push float 1 to stack
#define OPCODE_F2			0x08		// Push float 2 to stack
#define OPCODE_F3			0x09		// Push float 3 to stack
#define OPCODE_FCONST		0x0A		// Push float from constant pool to stack
#pragma enregion

#pragma region Char_Alloc_Ops
#define OPCODE_C0			0x0B		// Push char 0 to stack (false)
#define OPCODE_C1			0x0C		// Push char 1 to stack (true)
#define OPCODE_CCONST		0x0D		// Push char from constant pool to stack
#pragma endregion

#pragma region Other_Alloc_Ops
#define OPCODE_DCONST 		0x0E		// Push double from constant pool to stack
#define OPCODE_LCONST		0x0F		// Push long from constant pool to stack
#define OPCODE_SCONST		0x10		// Push short from constant pool to stack
#define OPCODE_STRCONST		0x11
#pragma endregion

#pragma region Char_Cons
#define OPCODE_CTOS			0x12
#define OPCODE_CTOI			0x13
#define OPCODE_CTOF			0x14
#define OPCODE_CTOL			0x15
#define OPCODE_CTOD			0x16
#pragma endregion

#pragma region Short_Convs
#define OPCODE_STOC			0x17
#define OPCODE_STOI			0x18
#define OPCODE_STOF			0x19
#define OPCODE_STOL			0x1A
#define OPCODE_STOD			0x1B
#pragma endregion

#pragma region Int_Convs
#define OPCODE_ITOC			0x1C
#define OPCODE_ITOS			0x1D
#define OPCODE_ITOF			0x1E
#define OPCODE_ITOL			0x1F
#define OPCODE_ITOD			0x20
#pragma endregion

#pragma region Float_Convs
#define OPCODE_FTOC			0x21
#define OPCODE_FTOS			0x22
#define OPCODE_FTOI			0x23
#define OPCODE_FTOL			0x24
#define OPCODE_FTOD			0x25
#pragma endregion

#pragma region Long_Convs
#define OPCODE_LTOC			0x26
#define OPCODE_LTOS			0x27
#define OPCODE_LTOI			0x28
#define OPCODE_LTOF			0x29
#define OPCODE_LTOD			0x2A
#pragma endregion

#pragma region Double_Convs
#define OPCODE_DTOC			0x2B
#define OPCODE_DTOS			0x2C
#define OPCODE_DTOI			0x2D
#define OPCODE_DTOF			0x2E
#define OPCODE_DTOL			0x2F
#pragma endregion

#pragma region Int_Ops
#define OPCODE_ADDI			0x30		// Add last integers on stack
#define OPCODE_SUBI			0x31		// Subdivide last integers on stack
#define OPCODE_MULI			0x32		// Multiply last integers on stack
#define OPCODE_DIVI			0x33		// Divide last integers on stack
#define OPCODE_CMPI			0x34		// Compare last integers on stack
#pragma endregion

#pragma region Float_Ops
#define OPCODE_ADDF			0x35		// Add last floats on stack
#define OPCODE_SUBF			0x36		// Subdivide last floats on stack
#define OPCODE_MULF			0x37		// Multiply last floats on stack
#define OPCODE_DIVF			0x38		// Divide last floats on stack
#define OPCODE_CMPF			0x39		// Compare last floats on stack
#pragma endregion

#pragma region Char_Ops
#define OPCODE_ADDC			0x3A		// Add last chars on stack
#define OPCODE_SUBC			0x3B		// Subdivide last chars on stack
#define OPCODE_MULC			0x3C		// Multiply last chars on stack
#define OPCODE_DIVC			0x3D		// Divide last chars on stack
#define OPCODE_CMPC			0x3E		// Compare last chars on stack
#pragma endregion

#pragma region Double_Ops
#define OPCODE_ADDD			0x3F		// Add last doubles on stack
#define OPCODE_SUBD			0x40		// Subdivide last doubles on stack
#define OPCODE_MULD			0x41		// Multiply last doubles on stack
#define OPCODE_DIVD			0x42		// Divide last doubles on stack
#define OPCODE_CMPD			0x43		// Compare last doubles on stack
#pragma endregion

#pragma region Long_Ops
#define OPCODE_ADDL			0x44		// Add last longs on stack
#define OPCODE_SUBL			0x45		// Subdivide last longs on stack
#define OPCODE_MULL			0x46		// Multiply last longs on stack
#define OPCODE_DIVL			0x47		// Divide last longs on stack
#define OPCODE_CMPL			0x48		// Compare last longs on stack
#pragma endregion

#pragma region Short_Ops
#define OPCODE_ADDS			0x49		// Add last shorts on stack
#define OPCODE_SUBS			0x4A		// Subdivide last shorts on stack
#define OPCODE_MULS			0x4B		// Multiply last shorts on stack
#define OPCODE_DIVS			0x4C		// Divide last shorts on stack
#define OPCODE_CMPS			0x4D		// Compare last shorts on stack
#pragma endregion

#pragma region String_Ops
#define OPCODE_CONCSTR		0x4E
#define OPCODE_COMPSTR		0x4F
#define OPCODE_DUPSTR		0x50
#pragma endregion

#pragma region Boolean_Ops
#define OPCODE_NEG			0x51		// Negate last boolean on stack (if 0 then 1; otherswhise 0)
#define OPCODE_AND			0x52		// Compare last booleans on stack for AND
#define OPCODE_OR			0x53		// Compare last booleans on stack for OR
#define OPCODE_EQ			0x54		// Compare last char on stack for EQ
#define OPCODE_LT			0x55		// Compare last char on stack for GT
#define OPCODE_GT			0x56		// Compare last char on stack for LT
#define OPCODE_LQ			0x57		// Compare last char on stack for GQ
#define OPCODE_GQ			0x58		// Compare last char on stack for LQ
#pragma endregion

#pragma region Jump_Ops
#define OPCODE_JMP			0x59		// Jump by offset from current position
#define OPCODE_JIF			0x5A		// Jump by offset from current position if last stack char is 0
#pragma endregion

#pragma region Storage_Ops
#define OPCODE_LSTORE		0x5B		// Store last value from stack in local scope at index
#define OPCODE_LLOAD		0x5C		// Load last value from local scope index to stack
#define OPCODE_OSTORE		0x5D		// Store last value from stack in last object at index
#define OPCODE_OLOAD		0x5E		// Load last value from object scope at index to stack
#define OPCODE_ASTORE		0x5F
#define OPCODE_ALOAD		0x60
#pragma endregion

#pragma region Function_Ops
#define OPCODE_CALL			0x61		// Call function by signature in constant pool at index
#define OPCODE_FLOAD		0x62		// Create object-wrapper around function by signature in constant pool at index
#define OPCODE_RETURN		0x63		// Discard current local scope, if following byte = 0, discard stack, else copy last value
#pragma endregion

#pragma region Array_Ops
#define OPCODE_ALEN			0x64
#define OPCODE_AFILL		0x65
#pragma endregion

#pragma region New_Ops
#define OPCODE_NEWO			0x66
#define OPCODE_NEWSTR		0x67
#define OPCODE_NEWARR		0x68
#pragma endregion

#pragma region Stack_Ops
#define OPCODE_POP			0x69
#define OPCODE_DUP			0x6A
#pragma endregion



#endif