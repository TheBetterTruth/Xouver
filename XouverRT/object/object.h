#ifndef OBJECT_H
#define OBJECT_H

#include <XNI.h>
#include <class/xclass.h>
#include <string>

struct Object : _xobject {
	const xClass& classObj;
	xValue* obj;
	
	Object(const xClass& classObj, xValue* obj) : classObj(classObj), obj(obj) {}
	~Object() {
		delete[] obj;
	}
};

#endif