#include "common.h"

#define XRT
#include <XNI.h>
#include <cstring>
#include <exception>
#include <fstream>
#include <iostream>
#include <string>

#include <opcodes.h>
#include <runtime/runtime.h>

#include <mapping/functionmap.h>
#include <memory/memory.h>

#include <class/classmanager.h>
#include <class/native/nativeclasses.h>
#include <class/xclass.h>

#include <memory/memory.h>

#define VERSION_MAJ 1
#define VERSION_MIN 0
#define XNI_VERIFY_NUM 0x2008

XNI_Error currentError = XNI_Error::NO_XNI_ERROR;
Runtime* rt;

bool isBig;

void* createRuntime(XNI_Error* error, unsigned char* bytes, int bytesCount) {
	if (bytes == nullptr) {
		*error = XNI_Error::FAILED_RT_CREATION;
		return nullptr;
	}

	Runtime* _rt = new Runtime();
	unsigned char* buffer =
			new unsigned char[sizeof(unsigned char*) * bytesCount];

	if (buffer == nullptr) {
		*error = XNI_Error::FAILED_RT_CREATION;
		return nullptr;
	}

	memcpy(buffer, bytes, bytesCount);
	_rt->bytes = buffer;

	registerFunction(_rt, "int:ToString()#void", *nativeInt::toString);
	registerFunction(_rt, "object:Equals()#void", *nativeObject::equals);

	*error = NO_XNI_ERROR;
	return rt;
}

Runtime* _convertRT(void* rt) {
	try {
		return ((Runtime*)rt);
	} catch (std::exception e) {
		currentError = XNI_Error::ARGUMENT_NOT_RT;
		return nullptr;
	}
}
#define convertRT(_rt)                   \
	if ((rt = _convertRT(_rt)) == nullptr) \
		XNI_Error::POINTER_TO_NO_RT;

XNI_Error createClass(void* _rt, unsigned char* buffer, int bytesCount) {
	convertRT(_rt);
	int ptr = 0;

	int n = 1;
	if (*(char*)&n == 1) isBig = false;
	else isBig = true;

	int classCount = BYTE_INT(buffer, &ptr);
	MemoryManager& mem = rt->getMemoryManager();
	FunctionMap& fmap = rt->getFunctionMap();

	while (classCount > 0) {
		xClass c{};
		int nameLen = BYTE_INT(buffer, &ptr);
		c.fullName = std::string(sizeof(char) * nameLen, '.');

		for (int i = 0; i < nameLen; i++)
			c.fullName[i] = buffer[ptr++];

		c.scopeSize = BYTE_INT(buffer, &ptr);
		c.functionsSize = BYTE_INT(buffer, &ptr);
		// unused c->functions = (int*)mem->allocate(sizeof(int));
		for (int i = 0; i < c.functionsSize; i++) {
			FunctionInfo info;

			int signatureLen = BYTE_INT(buffer, &ptr);
			info.signature = std::string(sizeof(char) * signatureLen, '.');
			for (int j = 0; j < signatureLen; j++)
				info.signature[j] = buffer[ptr++];

			info.signature[signatureLen] = '\0';
			info.pointer = BYTE_INT(buffer, &ptr);

			fmap.putFunction(info);
		}
		c.poolSize = BYTE_INT(buffer, &ptr);
		c.pool = std::vector<xValue>();
		for (int i = 0; i < c.poolSize; i++) {
			xValue val{};

			unsigned char type = buffer[ptr++];

			if (type == 11) {
				val.type = ValueType::INT;
				val.value.i = BYTE_INT(buffer, &ptr);
			} else if (type == 12) {
				val.type = ValueType::FLOAT;
				val.value.f = BYTE_FLOAT(buffer, &ptr);
			}
			else
				throw std::exception();

			c.pool.push_back(val);
		}
		int endOff = BYTE_INT(buffer, &ptr);
		c.funcsOffset = ptr;

		c.classObj = {};
		c.classObj.type = ValueType::OBJECT;
		Object& obj = rt->getMemoryManager().allocate(c);
		obj.obj = new xValue[c.scopeSize];

		c.classObj.value.o = &obj;
		rt->getClassManager().putClass(c);
		ptr += endOff;

		classCount--;
	}
	return NO_XNI_ERROR;
}

const char* resolveRTError(XRT_Error error) {
	switch (error) {
		case (XRT_Error::EXCEPTION_THROWN):
			return "Exception thrown";
		case (XRT_Error::MEMORY_ALLOCATION_FAILED):
			return "Memory allocation failed";
		case (XRT_Error::MEMORY_INITIALIZATION_FAILED):
			return "Memory initialization failed";
		default:
			return "No error";
	}
}
const char* resolveXNIError(XNI_Error error) {
	switch (error) {
		case (XNI_Error::ARGUMENT_NOT_RT):
			return "Passed argumend is not a pointer to runtime";
		case (XNI_Error::POINTER_TO_NO_RT):
			return "Passed argumend is not a pointer to runtime";
		case (XNI_Error::UNEXPECTED_BYTE):
			return "Given bytes contain unexpected order";
		case (XNI_Error::NO_XNI_ERROR):
			return "No error";
		default:
			return "Inalid XNI_Error argument";
	}
}

bool xniVerify(char c) {
	return (c == XNI_VERIFY_NUM);
}

int getXMajorVersion() {
	return VERSION_MAJ;
}

int getXMinorVersion() {
	return VERSION_MIN;
}

void registerFunction(void* _rt, const char* signature, void (*fn)(void*)) {
	convertRT(_rt);

	rt->putNativeFunction(std::string(signature), fn);
}

void callFunction(void* _rt, int index) {
	convertRT(_rt);

	rt->callFunction(index);
}

void runRuntime(void* _rt,
								const char* mainClass,
								const char* mainFuncSignature) {
	convertRT(_rt);

	xClass& c = rt->getClassManager().getClass(mainClass);
	rt->run(c, mainFuncSignature);
}

void haltRuntime(void* _rt) {
	convertRT(_rt);

	rt->halt();
}

void throwError(void* _rt, const char* msg) {
	convertRT(_rt);

	rt->throwError(msg);
}

int getExpceptionMessageSize(void* _rt) {
	convertRT(_rt);

	return rt->getException().size();
}

void getExceptionMessage(void* _rt, char** outMsg) {
	convertRT(_rt);

	for (int i = 0; i < rt->getException().size(); i++) {
		(*outMsg)[i] = rt->getException()[i];
	}
}

void freeRuntime(void* _rt) {
	convertRT(_rt);
	delete rt;
}

/*const xvalues& getStackTop(void* _rt) {
	convertRT(_rt);

	return rt->getStackTop();
}*/


int _byteint(unsigned char* bytes, int* pos) {
	unsigned char* buffer = new unsigned char[4];

	//if (*(char*)&n != 1) {
		buffer[0] = bytes[*pos];
		buffer[1] = bytes[*pos + 1];
		buffer[2] = bytes[*pos + 2];
		buffer[3] = bytes[*pos + 3];
	/*} else {
		buffer[0] = bytes[*pos + 3];
		buffer[1] = bytes[*pos + 2];
		buffer[2] = bytes[*pos + 1];
		buffer[3] = bytes[*pos];
	}*/

	*pos += 4;

	int val;
	memcpy(&val, buffer, sizeof(int));
	delete[] buffer;

	return val;
}

float _bytefloat(unsigned char* bytes, int* pos) {
	unsigned char* buffer = new unsigned char[4];

	float n = 1;
	if (*(char*)&n == 1) {
		buffer[0] = bytes[*pos];
		buffer[1] = bytes[*pos + 1];
		buffer[2] = bytes[*pos + 2];
		buffer[3] = bytes[*pos + 3];
	} else {
		buffer[0] = bytes[*pos + 3];
		buffer[1] = bytes[*pos + 2];
		buffer[2] = bytes[*pos + 1];
		buffer[3] = bytes[*pos];
	}

	*pos += 4;

	float val;
	memcpy(&val, buffer, sizeof(float));
	delete[] buffer;

	return val;
}