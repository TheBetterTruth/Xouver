# class_info<br>struct

## Declaration
```cpp
typedef struct {
	uint64_t package;
	char* class_name;
	char* class_path;

	uint16_t pool_size;
	uint64_t pool_ptr;

	uint16_t field_count;

	uint16_t function_count;
	char** function_signatures;
	uint64_t* function_ptrs;
} class_info;
```

## Description
Represents a class read from a package's byte code.

## Members
|Type|Name|Description|
|----|----|-----------|
|uint64_t|package|The address of the package this class belongs to.|
|char*|class_name|The short name of this class (eg. ``Int``).|
|char*|class_path|The path of this class (eg. ``Xouver.Lang``).|
|uint16_t|pool_size|The amount of constants used in this class.|
|uint64_t|pool_ptr|The position of the pool's first byte in the package's bytecode.<br><br>May be left empty, if this struct is used to create bytecode.|
|uint16_t|field_count|The amount of fields in this class.|
|uint16_t|function_count|The amount of functions in this class.|
|char**|function_signatures|An array containing all function signatures belonging to the functions of this class.|
|uint64_t|function_ptrs|An array containing all positions of this class' functions in the package's byte code.<br><br>Must be ordered according to ``function_signatures``|