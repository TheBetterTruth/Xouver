#include "xpkg.h"

#include "helpers.h"
#include "returncodes.h"

#include <stdint.h>
#include <string.h>

extern "C" {
	const unsigned char xpkg_min_version = 0;
	const unsigned char xpkg_maj_version = 1;

	unsigned char read_classes(const unsigned char* _Bytes, uint64_t& _Offset, package_info& _PackageInfo) {
		uint32_t class_count;
		TRY(read_uint32(_Bytes, _Offset, class_count));
		_PackageInfo.class_count = class_count;

		ALLOC(_PackageInfo.class_names, sizeof(char*) * class_count);
		ALLOC(_PackageInfo.class_ptrs, sizeof(uint64_t*) * class_count);

		char** class_names = _PackageInfo.class_names;
		uint64_t* class_ptrs = _PackageInfo.class_ptrs;

		for (uint32_t i = 0; i < class_count; i++) {
			uint32_t name_len;
			TRY(read_uint32(_Bytes, _Offset, name_len));

			char* name;
			ALLOC(name, sizeof(char) * (name_len + 1));

			for (unsigned char j = 0; j < name_len; j++) {
				name[i] = _Bytes[_Offset++];
			}
			name[name_len] = 0;

			_PackageInfo.class_names[i] = name;
			TRY(read_uint64(_Bytes, _Offset, _PackageInfo.class_ptrs[i]));
		}

		return XVR_SUCCESS;
	}
	
	unsigned char validate(const unsigned char* _Bytes, uint64_t& _Offset) {
		uint32_t num;
		TRY(read_uint32(_Bytes, _Offset, num));

		char buf[4];
		for (int i = 0; i < 4; i++) {
			buf[i] = num % 10;
			num /= 10;
		}

		if (strcmp(buf, "xpkg") == 0)
			return XVR_SUCCESS;
		else
			return XPKG_INVALID_PACKAGE;
	}
	
	unsigned char xpkg_get_package(const unsigned char* _Bytes, package_info* _PackageInfo) {
		if (_Bytes == nullptr) return XVR_NULLPTR;
		
		uint64_t offset = 0;

		TRY(validate(_Bytes, offset));

		_PackageInfo->handle = (uint64_t) _Bytes;
		_PackageInfo->minor_version = _Bytes[offset++];
		_PackageInfo->major_version = _Bytes[offset++];

		return read_classes(_Bytes, offset, *_PackageInfo);
	}

	unsigned char xpkg_get_class(const package_info* _PackageInfo, const uint32_t _Index, class_info* _ClassInfo) {
		if (_PackageInfo == nullptr) return XVR_NULLPTR;
		if (_Index >= _PackageInfo->class_count) return XVR_INDEX_OUT_OF_BOUNDS;

		uint64_t offset = _PackageInfo->class_ptrs[_Index];
		const unsigned char* bytes = (const unsigned char*)_PackageInfo->handle;
		
		_ClassInfo->package = (uint64_t) _PackageInfo;
		
		unsigned char name_len = bytes[offset++];
		_ClassInfo->class_name;
		ALLOC(_ClassInfo->class_name, sizeof(char) * (name_len+1));
		char* class_name = _ClassInfo->class_name;

		for (int i = 0; i < name_len; i++)
			class_name[i] = bytes[offset++];
		class_name[name_len] = 0;

		uint16_t path_len;
		TRY(read_uint16(bytes, offset, path_len));
		ALLOC(_ClassInfo->class_path, sizeof(char) * (path_len+1));
		char* class_path = _ClassInfo->class_path;

		for (uint16_t i = 0; i < path_len; i++)
			class_path[i] = bytes[offset++];
		class_path[path_len] = 0;

		TRY(read_uint16(bytes, offset, _ClassInfo->pool_size));
		TRY(read_uint64(bytes, offset, _ClassInfo->pool_ptr));
		TRY(read_uint16(bytes, offset, _ClassInfo->field_count));

		uint16_t function_count;
		TRY(read_uint16(bytes, offset, function_count));
		_ClassInfo->function_count = function_count;

		ALLOC(_ClassInfo->function_signatures, sizeof(char*) * _ClassInfo->function_count);
		ALLOC(_ClassInfo->function_ptrs, sizeof(uint64_t) * _ClassInfo->function_count);
		char** function_signatures = _ClassInfo->function_signatures;
		uint64_t* function_ptrs = _ClassInfo->function_ptrs;

		for (uint16_t i = 0; i < function_count; i++) {
			uint32_t signature_len;
			TRY(read_uint32(bytes, offset, signature_len));

			char* signature;
			ALLOC(signature, sizeof(char) * (signature_len+1));

			for (uint32_t j = 0; j < signature_len; j++)
				signature[i] = bytes[offset++];
			signature[signature_len] = 0;

			_ClassInfo->function_signatures[i] = signature;
			TRY(read_uint64(bytes, offset, function_ptrs[i]));
		}

		return XVR_SUCCESS;
	}

	unsigned char xpkg_find_class(const package_info* _PackageInfo, const char* _ClassName, uint32_t* _ClassIndex) {
		if (_PackageInfo == nullptr) return XVR_NULLPTR;

		uint32_t class_count = _PackageInfo->class_count;
		for (uint32_t i = 0; i < class_count; i++) {
			if (strcmp(_PackageInfo->class_names[i], _ClassName)) {
				*_ClassIndex = i;
				return XVR_SUCCESS;
			}
		}

		return XPKG_CLASS_NOT_FOUND;
	}
	unsigned char xpkg_get_constant(const class_info* _ClassInfo, const uint16_t _Index, constant_info* _ConstantInfo) {
		if (_ClassInfo == nullptr) return XVR_NULLPTR;
		if (_Index >= _ClassInfo->pool_size) return XVR_INDEX_OUT_OF_BOUNDS;

		const unsigned char* bytes = (unsigned char*) ((package_info*)_ClassInfo->package)->handle;
		uint64_t pool_ptr = _ClassInfo->pool_ptr;
		uint64_t offset = _Index * sizeof(uint64_t);
		offset = pool_ptr + offset;
		_ConstantInfo->type = bytes[offset++];
		
		uint64_t size;
		TRY(read_uint64(bytes, offset, size));
		_ConstantInfo->size = size;

		ALLOC(_ConstantInfo->bytes, size);
		unsigned char* _bytes = _ConstantInfo->bytes;
		for (uint64_t i = 0; i < size; i++) {
			_bytes[i] = bytes[offset++];
		}

		return XVR_SUCCESS;
	}

	unsigned char xpkg_get_function(const class_info* _ClassInfo, const uint16_t _Index, function_info* _FunctionInfo) {
		if (_ClassInfo == nullptr) return XVR_NULLPTR;
		if (_Index >= _ClassInfo->function_count) return XVR_INDEX_OUT_OF_BOUNDS;

		const unsigned char* bytes = (unsigned char*)((package_info*)_ClassInfo->package)->handle;
		size_t offset = _ClassInfo->function_ptrs[_Index];

		_FunctionInfo->param_count = bytes[offset++];
		_FunctionInfo->scope_size = bytes[offset++];
		
		uint16_t instruction_count;
		TRY(read_uint16(bytes, offset, instruction_count));
		_FunctionInfo->instruction_count = instruction_count;

		ALLOC(_FunctionInfo->instructions, sizeof(instruction) * instruction_count);
		instruction* instructions = _FunctionInfo->instructions;

		for (uint16_t i = 0; i < instruction_count; i++) {
			instruction& instruct = instructions[i];

			instruct.opcode = bytes[offset++];

			unsigned char args_count = bytes[offset++];
			instruct.arg_count = args_count;

			if (args_count > 0) {
				ALLOC(instruct.args, sizeof(uint16_t) * args_count);
				uint16_t* args = instruct.args;

				for (unsigned char j = 0; j < args_count; j++) {
					TRY(read_uint16(bytes, offset, args[j]));
				}
			}
		}

		return XVR_SUCCESS;
	}
	unsigned char xpkg_find_function(const class_info* _ClassInfo, const char* _Signature, uint16_t* _FunctionIndex) {
		uint16_t function_count = _ClassInfo->function_count;

		const char** signatures = _ClassInfo->function_signatures;
		for (uint16_t i = 0; i < function_count; i++) {
			if (strcmp(signatures[i], _Signature) == 0) {
				*_FunctionIndex = i;
				return XVR_SUCCESS;
			}
		}

		return XPKG_FUNCTION_NOT_FOUND;
	}
}