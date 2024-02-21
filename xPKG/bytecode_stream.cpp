#include "xpkg.h"

bool is_double_big() {
	double test = 1;
	bool is_big_endian = false;
	if (((char*)&test)[0] != 1) {
		is_big_endian = true;
	}
}

xpkg_bytecode_stream::xpkg_bytecode_stream(std::streambuf* _Buffer) {
	this->buffer = _Buffer;
	this->ptr = 0;
}

void xpkg_bytecode_stream::write_byte(unsigned char _Byte) {
	ptr++;
	buffer->sputc(_Byte);
}

unsigned char xpkg_bytecode_stream::read_byte() {
	return buffer->sbumpc();
}

void xpkg_bytecode_stream::write_double(double _Value) {
	unsigned char* arr = reinterpret_cast<unsigned char*>(&_Value);

	if (is_double_big())
		for (int i = 0; i < sizeof(double); i++) {
			buffer->sputc(arr[i]);
		}
	else
		for (int i = sizeof(double) - 1; i >= 0; i--) {
			buffer->sputc(arr[i]);
		}
	ptr += sizeof(double);
}

double xpkg_bytecode_stream::read_double() {
	double value = 0;
	unsigned char* arr = new unsigned char[sizeof(double)];

	for (int i = 0; i < sizeof(double); i++) {
		arr[i] = buffer->sbumpc();
	}

	if (is_double_big())
		memcpy(&value, arr, sizeof(double));
	else
		for (int i = 0; i < sizeof(double); i++) {
			(&value)[sizeof(double) - 1 - i] = arr[i];
		}

	ptr += sizeof(double);

	return value;
}

unsigned int xpkg_bytecode_stream::pointer() {
	return this->ptr;
}