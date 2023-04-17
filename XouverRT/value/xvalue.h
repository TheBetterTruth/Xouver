#ifndef XVALUE_H
#define XVALUE_H

#include <XNI.h>
#include "ValueType.h"

struct xValue {
	ValueType type;
	xvalues value;

	bool operator == (xValue v) {
		xvalues val = v.value;

		if (v.type == this->type) {
			if (this->type == ValueType::FLOAT)
				return (val.f == value.f);
			else if (this->type == ValueType::INT)
				return (val.i == value.i);
			else if (this->type == ValueType::BOOL)
				return (val.b == value.b);
			else if (this->type == ValueType::CHAR)
				return (val.c == value.c);
			else if (this->type == ValueType::OBJECT)
				return (val.o == value.o);
			else
				return false;
		}

		return false;
	}
};

#endif