# instruction<br>struct

## Declaration
```c
typedef struct {
	unsigned char opcode;
	unsigned char arg_count;
	uint16_t* args;
} instruction;
```

## Members
|Type|Name|Description|
|----|----|-----------|
|unsigned char|opcode|The opcode of this instruction.<br><br>See [opcodes](../opcodes.md).|
|unsigned char|arg_count|The amount of arguments this instruction has.|
|uint16_t*|args|An array of the arguments of this instruction.|