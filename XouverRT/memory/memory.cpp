#include "memory.h"

#include <XNI.h>

#include <runtime/runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

static const std::size_t slabSize = 1 << 19;
static const std::size_t youngGenerationSize = 16;

MemoryManager::MemoryManager(const void* rt) {
	this->rt = rt;
}

Object& MemoryManager::allocate(const xClass& classObj) {
	auto& memory = getYoung();
	if (memory.size() == youngGenerationSize) {
		auto rt = (Runtime*)this->rt;
		rt->fullStack();
		/* GC */
	}
	memory.push_back({ classObj, new xValue[classObj.scopeSize]});	 // TODO: Object creation
	return memory[memory.size() - 1];
}

MemoryManager::~MemoryManager() {}

MemoryManager::Memory& MemoryManager::getYoung() {
	return isYoungA ? youngA : youngB;
}