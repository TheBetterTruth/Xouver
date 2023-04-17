#ifndef CLASS_MANAGER_H
#define CLASS_MANAGER_H

#include <string>
#include <vector>

#include "xclass.h"

class ClassManager {
 private:
	std::vector<xClass> classes;

 public:
	void putClass(xClass c);
	xClass& getClass(int index);
	xClass& getClass(std::string path);
};

#endif