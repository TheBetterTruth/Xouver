#ifndef OP_CODES_H
#define OP_CODES_H

#define OP_ADD				(unsigned char) 1
#define OP_SUB				(unsigned char) 2
#define OP_MUL				(unsigned char) 3
#define OP_DIV				(unsigned char) 4
#define OP_JMP				(unsigned char) 5
#define OP_IFEQ				(unsigned char) 6
#define OP_IFGT				(unsigned char) 7
#define OP_IFGQ				(unsigned char) 9
#define OP_NOT				(unsigned char) 11
#define OP_JIF				(unsigned char) 12
#define OP_CALL				(unsigned char) 13	
#define OP_RET0				(unsigned char) 14
#define OP_RET1				(unsigned char) 15
#define OP_YIELD			(unsigned char) 16
#define OP_THROW			(unsigned char) 17

#define OP_CLOAD			(unsigned char) 18
#define OP_LOAD				(unsigned char) 19
#define OP_STORE			(unsigned char) 20

#define OP_NEWO				(unsigned char) 21
#define OP_OLOAD			(unsigned char) 22
#define OP_OSTORE			(unsigned char) 23
#define OP_INVO				(unsigned char) 24
#define OP_NEW				(unsigned char) 25

#define OP_POP				(unsigned char) 26
#define OP_CLS				(unsigned char) 27

#define OP_LINE				(unsigned char) 28

#define OP_NEWA				(unsigned char) 29
#define OP_ASTORE			(unsigned char) 30
#define OP_ALOAD			(unsigned char) 31
#define OP_APPA				(unsigned char) 32
#define OP_ARRLEN			(unsigned char) 33

#define OP_PUSHC			(unsigned char) 34
#endif