#ifndef CLASSMAP_H
#define CLASSMAP_H

#include <vector>

struct ClassInfo {
	char* signature;
	int pointer;
};

class ClassMap {
private:
	std::vector<ClassInfo> classes;
public:
	ClassInfo& getClass(int index);
	void putClass(ClassInfo info);
};

#endif