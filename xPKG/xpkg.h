#ifndef XPKG_H
#define XPKG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	const extern unsigned char xpkg_minor_version;
	const extern unsigned char xpkg_major_version;

	typedef struct {
		unsigned char opcode;
		unsigned char arg_count;
		uint16_t* args;
	} instruction;

	typedef struct {
		unsigned char param_count;
		uint16_t scope_size;

		uint16_t instruction_count;
		instruction* instructions;
	} function_info;

	typedef struct {
		unsigned char type;
		uint32_t size;
		unsigned char* bytes;
	} constant_info;

	typedef struct {
		uint64_t package;
		char* class_name;
		char* class_path;

		uint16_t pool_size;
		uint64_t pool_ptr;

		uint16_t field_count;

		uint16_t function_count;
		char** function_signatures;
		uint64_t* function_ptrs;
	} class_info;

	typedef struct {
		unsigned char minor_version;
		unsigned char major_version;

		unsigned char package_type;

		uint64_t handle;

		uint32_t class_count;

		char** class_names;
		uint64_t* class_ptrs;
	} package_info;

	unsigned char xpkg_get_package(const unsigned char* _Bytes, package_info* _PackageInfo);
	unsigned char xpkg_get_class(const package_info* _PackageInfo, const uint32_t _Index, class_info* _ClassInfo);
	unsigned char xpkg_find_class(const package_info* _PackageInfo, const char* _ClassName, uint32_t* _ClassIndex);
	unsigned char xpkg_get_constant(const class_info* _ClassInfo, const uint16_t _Index, constant_info* _ConstantInfo);
	unsigned char xpkg_get_function(const class_info* _ClassInfo, const uint16_t _Index, function_info* _FunctionInfo);
	unsigned char xpkg_find_function(const class_info* _ClassInfo, const char* _Signature, uint32_t* _FunctionIndex);
#ifdef __cplusplus
}
#endif

#endif