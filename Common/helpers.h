#ifndef HELPERS_H
#define HERLPERS_H

#include <memory>
#include "returncodes.h"

bool is_big_int;
bool is_big_float;
bool is_big_llong;

void test_endian() {
	int i = 1;
	is_big_int = *(char*)&i == 1;

	float f = 1;
	is_big_float = *(char*)&f == 1;
}

template <typename T>
unsigned char read(const unsigned char* _Bytes, size_t& _Offset, bool _ShouldReverse, T& _Out) {
	size_t size = sizeof(T);
	ALLOC(buf, size);

	for (int i = 0; i < size; i++) {
		buf[i] = _Bytes[_Offset++];
	}

	if (_ShouldReverse) {
		for (int i = 0; i < size - 1; i++) {
			auto current = buf[i];
			buf[i] = buf[i + 1];
			buf[i + 1] = buf[i];
		}
	}

	T out = *(T*)buf;
	free(buf);

	_Out = out;

	return XPKG_SUCCESS;
}

inline unsigned char read_uint16(const unsigned char* _Bytes, size_t& _Offset, uint16_t& _Out) {
	return read<uint16_t>(_Bytes, _Offset, !is_big_int, _Out);
}

inline unsigned char read_uint32(const unsigned char* _Bytes, size_t& _Offset, uint32_t& _Out) {
	return read<uint32_t>(_Bytes, _Offset, !is_big_int, _Out);
}

inline unsigned char read_uint64(const unsigned char* _Bytes, size_t& _Offset, uint64_t& _Out) {
	return read<uint64_t>(_Bytes, _Offset, !is_big_int, _Out);
}

inline unsigned char read_size(const unsigned char* _Bytes, size_t& _Offset, size_t& _Out) {
	return read<size_t>(_Bytes, _Offset, !is_big_int, _Out);
}

inline unsigned char read_int32(const unsigned char* _Bytes, size_t& _Offset, int32_t& _Out) {
	return read<int32_t>(_Bytes, _Offset, !is_big_int, _Out);
}

inline float read_float(const unsigned char* _Bytes, size_t& _Offset, float& _Out) {
	return read<float>(_Bytes, _Offset, !is_big_float, _Out);
}

inline double read_double(const unsigned char* _Bytes, size_t& _Offset, double& _Out) {
	return read<double>(_Bytes, _Offset, !is_big_float, _Out);
}

#endif