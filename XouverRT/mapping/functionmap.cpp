#include "functionmap.h"

#include <memory/memory.h>

FunctionInfo& FunctionMap::getFunction(int index) {
	return functions[index];
}


FunctionInfo& FunctionMap::getFunction(std::string signature) {
	for (auto& f : functions)
		if (f.signature == signature)
			return f;
}

void FunctionMap::putFunction(FunctionInfo info) {
	functions.push_back(info);
}