#include "classmanager.h"
#include "xclass.h"
#include <string>

xClass& ClassManager::getClass(int index) {
	return classes[index];
}

xClass& ClassManager::getClass(std::string path) {
	for (auto &it : classes) {
		if (it.fullName == path)
			return it;
	}
}

void ClassManager::putClass(xClass c) {
	classes.push_back(c);
}
