#include "Runtime.h"

#include <XNI.h>
#include <iostream>
#include <string>
#include <memory/memory.h>
#include <common.h>
#include <Object/Object.h>
#include <opcodes.h>
#include <logging.h>


#define BYTES_END (unsigned char)0x94

#define MAP_BEGIN (unsigned char)0x74
#define POOL_BEGIN (unsigned char)0x33

Runtime::Runtime() : memManager{this} {}

FunctionMap& Runtime::getFunctionMap() {
	return functionmap;
}

Runtime::~Runtime() {
	delete[] this->bytes;
}

void Runtime::run(xClass& mainClass, std::string func) {
	_isHalted = false;
	setClass(mainClass);
	callFunction(0);

	while (!_isHalted) {
		unsigned char inst = advance();
		debugPrint("Stack size: #\n", stack().size());
		switch (inst) {
			case OP_ADD: {
				xValue val2 = stackPop();
				xValue val1 = stackPop();

				debugPrint("INT: # + #\n", val1.value.i, val2.value.i);
				debugPrint("FLOAT: # + #\n", val1.value.f, val2.value.f);
				switch (val1.type) {
					case ValueType::FLOAT: {
						float v1 = val1.value.f;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.f = v1 + val2.value.f;
								v.type = ValueType::FLOAT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.i = v1 + val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					case ValueType::INT: {
						int v1 = val1.value.i;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.i = v1 + val2.value.f;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.i = v1 + val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					default:
						throw std::exception();
				}
				break;
			}
			case OP_SUB: {
				xValue val2 = stackPop();
				xValue val1 = stackPop();

				switch (val1.type) {
					case ValueType::FLOAT: {
						float v1 = val1.value.f;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.f = v1 - val2.value.f;
								v.type = ValueType::FLOAT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.f = v1 - val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					case ValueType::INT: {
						int v1 = val1.value.i;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.i = v1 - val2.value.f;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.i = v1 - val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					default:
						throw std::exception();
				}
				break;
			}
			case OP_MUL: {
				xValue val2 = stackPop();
				xValue val1 = stackPop();

				switch (val1.type) {
					case ValueType::FLOAT: {
						float v1 = val1.value.f;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.f = v1 * val2.value.f;
								v.type = ValueType::FLOAT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.i = v1 * val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					case ValueType::INT: {
						int v1 = val1.value.i;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.i = v1 * val2.value.f;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.i = v1 * val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					default:
						throw std::exception();
				}
				break;
			}
			case OP_DIV: {
				xValue val2 = stackPop();
				xValue val1 = stackPop();

				switch (val1.type) {
					case ValueType::FLOAT: {
						float v1 = val1.value.f;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.f = v1 / val2.value.f;
								v.type = ValueType::FLOAT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.i = v1 / val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					case ValueType::INT: {
						int v1 = val1.value.i;
						xValue v{};

						switch (val2.type) {
							case ValueType::FLOAT:
								v.value.i = v1 / val2.value.f;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							case ValueType::INT:
								v.value.i = v1 / val2.value.i;
								v.type = ValueType::INT;
								stackPush(v);
								break;
							default:
								throw std::exception();
						}
						break;
					}
					default:
						throw std::exception();
				}
				break;
			}
			case OP_CLOAD: {
				int pos = ptrs.top();
				int constPos = getArg();
				xValue v = currentClass->pool[constPos];

				if (v.type == ValueType::INT)
					debugPrint("CLOAD # from # on #\n", v.value.i, constPos, pos);
				else if (v.type == ValueType::FLOAT)
					debugPrint("CLOAD # from # on #\n", v.value.f, constPos, pos);

				stackPush(v);
				break;
			}
			case OP_LINE: {
				lines.top() = advance();
				break;
			}
			case OP_CALL: {
				int funcPtr = getArg();

				callFunction(funcPtr);
				break;
			}
			case OP_RET: {
				ptrs.pop();
				functionCallVector.pop_back();

				if (ptrs.size() == 0)
					_isHalted = true;
				break;
			}
			case OP_JMP: {
				int ptr = ptrs.top();
				int pos = getArg();

				if (pos < 0)
					ptrs.top() = ptr + pos;
				else
					ptrs.top() += pos - 1;
				break;
			}
			case OP_IFEQ: {
				xValue rval = stackPop();
				xValue lval = stackPop();

				xValue v{};
				v.type = ValueType::BOOL;
				if (rval.type == lval.type) {
					switch (lval.type) {
						case ValueType::INT:
							v.value.b = lval.value.i == rval.value.i;
							// std::cout << v->value.b << "\n";
							break;
						case ValueType::FLOAT:
							v.value.b = lval.value.f == rval.value.f;
							break;
						case ValueType::BOOL:
							v.value.b = lval.value.b == rval.value.b;
							break;
						case ValueType::CHAR:
							v.value.b = lval.value.c == rval.value.c;
							break;
						case ValueType::OBJECT:
							v.value.b = lval.value.c == rval.value.c;
							break;
						default:
							v.value.b = false;
							break;
					}
				} else
					v.value.b = false;

				stackPush(v);
				break;
			}
			case OP_IFGT: {
				xValue rval = stackPop();
				xValue lval = stackPop();
				debugPrint("# > #\n", lval.value.i, rval.value.i);

				xValue v{};
				v.type = ValueType::BOOL;
				if (rval.type == lval.type) {
					switch (lval.type) {
						case ValueType::INT:
							v.value.b = lval.value.i > rval.value.i;
							break;
						case ValueType::FLOAT:
							v.value.b = lval.value.f > rval.value.f;
							break;
						default:
							v.value.b = false;
							break;
					}
				} else
					v.value.b = false;

				stackPush(v);
				break;
			}
			case OP_IFGQ: {
				xValue rval = stackPop();
				xValue lval = stackPop();
				debugPrint("# >= #\n", lval.value.i, rval.value.i);

				xValue v = xValue();
				v.type = ValueType::BOOL;
				if (rval.type == lval.type) {
					switch (lval.type) {
						case ValueType::INT:
							v.value.b = lval.value.i >= rval.value.i;
							break;
						case ValueType::FLOAT:
							v.value.b = lval.value.f >= rval.value.f;
							break;
						default:
							v.value.b = false;
							break;
					}
				} else
					v.value.b = false;

				stackPush(v);
				break;
			}
			case OP_NOT: {
				xValue b = stackPop();

				if (b.type != ValueType::BOOL)
					break;

				xValue r = xValue();
				r.type = ValueType::BOOL;
				r.value.b = !b.value.b;

				stackPush(r);
				break;
			}
			case OP_JIF: {
				int pos = getArg();
				xValue b = stackPop();

				if (b.type != ValueType::BOOL)
					break;
				if (b.value.b != true)
					ptrs.top() += pos;
				break;
			}
			case OP_NEWO: {
				/*xClass* c = ((Runtime*)rt)->getClass(advance());
				Object* o = (Object*) allocate(sizeof(Object));

				int scopeSize = c->getScopeSize();
				o->classObj = c;
				o->scopeSize = scopeSize;
				o->obj = (xValue**)allocate(sizeof(xValue*) * scopeSize);

				xValue* v = (xValue*) allocate(sizeof(xValue));
				v->type = ValueType::OBJECT;
				v->value.o = o;

				stackPush(v);*/
				break;
			}
			case OP_OLOAD: {
				int i = advance();
				xValue o = getStackTop();

				if (o.type != ValueType::OBJECT)
					break;

				stackPush(((Object*)o.value.o)->obj[i]);
				break;
			}
			case OP_OSTORE: {
				int i = advance();
				xValue v = getStackTop();
				xValue o = getStackTop();

				if (o.type != ValueType::OBJECT)
					break;

				// ((Object*)o.value.b)->obj[i].value.o->refcount--;
				((Object&)o.value.b).obj[i] = v;
				break;
			}
			case OP_INVO: {
				/*xValue* o = stack().front();

				if (o->type != ValueType::OBJECT) break;

				xClass* c = ((Object*)o->value.o)->classObj;
				setClass(c);*/

				int idx = getArg();
				xClass& c = classManager.getClass(idx);
				setClass(c);

				debugPrint("INVOKE # on #\n", idx, ptrs.top());
				break;
			}
			case OP_NEW: {
				int idx = getArg();

				xValue v = xValue();
				v.type = ValueType::OBJECT;

				xClass& c = classManager.getClass(idx);

				Object& obj = getMemoryManager().allocate(c);

				v.value.o = &obj;
				stackPush(v);
				break;
			}
			case OP_POP: {
				int amount = advance();

				for (int i = 0; i < amount; i++) {
					stackPop();
				}

				break;
			}
			case OP_LOAD: {
				int pos = getArg();
				xValue val = localScopes.top()[pos];

				debugPrint("LOAD # in # on #\n", val.value.i, pos, ptrs.top());

				stackPush(val);
				break;
			}
			case OP_STORE: {
				int pos = getArg();
				xValue val = stackPop();

				debugPrint("STORE # at # on #\n", val.value.i, pos, ptrs.top());

				// val->refcount++;
				/*if (((Runtime*)rt)->scope[pos] != nullptr)
					((Runtime*)rt)->scope[pos]->refcount--;*/
				stack()[pos] = val;
				break;
			}
			case OP_PUSHC: {
				int idx = getArg();

				xClass& c = classManager.getClass(idx);
				debugPrint("PUSHC # on #", idx, ptrs.top());
				stack().push_back(c.classObj);
				break;
			}
			case OP_CLS: {
				while (stack().size() > 0)
					stackPop();
				break;
			}
			/*case OP_NEWA: {
				int len = getArg();

				xValue val{};

				val.type == ValueType::OBJECT;
				
				Object& array = getMemoryManager().allocate();
				array.obj = new xValue[len];

				val.value.o = static_cast<xobject>(&array);
				break;
			}
			case OP_ASTORE: {
				int idx = getArg();
				xValue arr = stackPop();
				xValue val = stackPop();

				if (idx == -1)
					idx = stackPop().value.i;

				Object& obj = (Object&) arr.value.o;
				obj.obj[1 + idx] = val;
				break;
			}
			case OP_ALOAD: {
				int idx = getArg();
				xValue arr = stackPop();

				if (idx == -1)
					idx = stackPop().value.i;

				Object* o = (Object*)arr.value.o;
				stackPush(o->obj[1 + idx]);
				break;
			}*/
			case 0:
				_isHalted = true;
				break;
			default:
				unsigned char inst = bytes[ptrs.top()];
				std::cout << "Unknown opcode: " << inst << "\n";
				_isHalted = true;
				break;
		}
	}

	_isHalted = true;
}

XRT_Error Runtime::getError() {
	return currentError;
}

void Runtime::setError(XRT_Error error) {
	this->currentError = error;
}

std::string Runtime::getException() {
	return this->exception;
}

void Runtime::halt() {
	_isHalted = true;
}

void Runtime::putNativeFunction(std::string signature, void (*fn)(void*)) {
	nativeFunctions.insert(
			std::pair<std::string, void(_xcdecl*)(void*)>(signature, fn));
}

void Runtime::setClass(xClass& c) {
	classes.push(&c);
	currentClass = &c;
}

void Runtime::callFunction(int id) {
	FunctionInfo info = functionmap.getFunction(id);
	std::string signature = info.signature;

	if (nativeFunctions.contains(signature)) {
		this->functionCallVector.push_back(signature.c_str());
		nativeFunctions[signature](this);
		this->functionCallVector.pop_back();
	} else {
		this->functionCallVector.push_back(signature.c_str());

		int ptr = currentClass->funcsOffset + info.pointer;
		Stack scope;
		int scopeSize = BYTE_INT(bytes, &ptr);
		scope = Stack{(std::size_t)scopeSize};

		int argsCount = BYTE_INT(bytes, &ptr);
		for (int i = argsCount - 1; i >= 0; i--) {
			scope[i] = stack().back();
			stack().pop_back();
		}
		ptrs.push(ptr - 1);

		localScopes.push(scope);
	}
}

xValue& Runtime::getStackTop() {
	return stack().back();
}

void Runtime::stackPush(xValue v) {
	stack().push_back(v);
}

xValue Runtime::stackPop() {
	auto& v = getStackTop();
	stack().pop_back();
	return v;
}

std::string Runtime::createCallStack() {
	std::string str = "";

	for (auto it = functionCallVector.rbegin(); it != functionCallVector.rend();
			 it++) {
		str += "  In.. " + *it + "\n";
	}

	return str;
}

std::string Runtime::createExceptionMessage(std::string msg) {
	bool isEmpty = msg.empty();

	if (!isEmpty) {
		isEmpty = true;
		for (auto it = msg.begin(); it != msg.end(); it++) {
			if (!std::isspace(*it)) {
				isEmpty = false;
				break;
			}
		}
	}

	if (isEmpty)
		msg = "NativeException:";

	return "Xouver Exception: " + msg + '\n' + createCallStack();
}

void Runtime::throwError(std::string msg) {
	this->exception = createExceptionMessage(msg);

	setError(XRT_Error::EXCEPTION_THROWN);
	haltRuntime(this);
}

ClassManager& Runtime::getClassManager() {
	return classManager;
}

MemoryManager& Runtime::getMemoryManager() {
	return memManager;
}

unsigned char Runtime::advance() {
	int ptr = ptrs.top() + 1;
	ptrs.pop();
	ptrs.push(ptr);

	unsigned char byte = bytes[ptr];
	return bytes[ptr];
}

int Runtime::getArg() {
	advance();
	int arg = BYTE_INT(bytes, &ptrs.top());

	ptrs.top()--;
	return arg;
}

Runtime::Stack& Runtime::stack() {
	return this->localScopes.top();
}

std::stack<Runtime::Stack>& Runtime::fullStack() {
	return this->localScopes;
}