#include "functionmap.h"

#include <memory/memory.h>

FunctionInfo& FunctionMap::getFunction(int index) {
	return functions[index];
}

void FunctionMap::putFunction(FunctionInfo info) {
	functions.push_back(info);
}