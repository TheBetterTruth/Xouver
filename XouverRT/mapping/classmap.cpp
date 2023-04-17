#include "classmap.h"

#include <memory/memory.h>

ClassInfo& ClassMap::getClass(int index) {
	return classes[index];
}

void ClassMap::putClass(ClassInfo info) {
	classes.push_back(info);
}