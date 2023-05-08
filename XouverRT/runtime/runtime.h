#ifndef RUNTIME_H
#define RUNTIME_H

#include <XNI.h>
#include <class/classmanager.h>
#include <class/xclass.h>

#include <mapping/classmap.h>
#include <mapping/functionmap.h>

#include <memory/memory.h>

#include <map>
#include <span>
#include <stack>
#include <string>

#ifdef __GNUC__
#define _xcdecl __attribute__((__cdecl__))
#else
#define _xcdecl __cdecl
#endif

class Runtime {
 private:
	using Stack = std::vector<xValue>;
	XRT_Error currentError;

	MemoryManager memManager;
	ClassManager classManager;

	bool _isHalted;
	xClass* currentClass;

	std::stack<int> ptrs;
	std::vector<std::string> functionCallVector;
	std::stack<xClass*> classes;
	std::stack<Stack> localScopes;
	std::stack<int> lines;

	std::map<std::string, void(_xcdecl*)(void*)> nativeFunctions;

	FunctionMap functionmap;
	ClassMap classmap;

	std::string exception;

 public:
	int funcOffset;
	unsigned char* bytes;

	Runtime();
	~Runtime();
	void run(xClass& mainClass, std::string func);

	XRT_Error getError();
	void setError(XRT_Error);

	std::string getException();

	void putNativeFunction(std::string signature, void (*fn)(void*));
	void setClass(xClass& c);
	void callFunction(int id);
	void callFunction(std::string signature);
	void halt();
	std::string createCallStack();
	std::string createExceptionMessage(std::string msg);
	void throwError(std::string msg);

	ClassManager& getClassManager();
	MemoryManager& getMemoryManager();
	FunctionMap& getFunctionMap();
	xValue& getStackTop();
	void stackPush(xValue v);
	xValue stackPop();
	Stack& stack();
	std::stack<Stack>& fullStack();

	unsigned char advance();
	int getArg();
};

#endif