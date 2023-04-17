#ifndef CONSTANT_H
#define CONSTANT_H

enum ConstantType {
	C_INT = 11,
	C_FLOAT = 12
};

struct Constant {
	const ConstantType type;
	const union Value {
		const int i;
		const float f;

		Value(const int& i) : i(i) {}
		Value(const float& f) : f(f) {}
	} value;

	Constant(const int& value) : type(ConstantType::C_INT), value(value) {}
	Constant(const float& value) : type(ConstantType::C_FLOAT), value(value) {}
};

#endif