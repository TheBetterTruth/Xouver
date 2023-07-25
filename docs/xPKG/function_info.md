# function_info<br>struct

## Declaration
```c
typedef struct {
	unsigned char param_count;
	uint16_t scope_size;

	uint16_t instruction_count;
	instruction* instructions;
} function_info;
```

## Members
|Type|Name|Description|
|----|----|-----------|
|unsigned char|param_count|The count of arguments this function accepts|
|uint16_t|scope_size|The size of this function's scope, i.e. the amount of variables defined throughout all child-scopes.|
|uint16_t|instruction_count|The amount of instructions this function consists of.|
|[instruction]*|instructions|An array of the instructions belonging to this function.|

[instruction]: instruction.md