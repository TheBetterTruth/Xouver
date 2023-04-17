#ifndef FUNCTIONMAP_H
#define FUNCTIONMAP_H

#include <string>
#include <vector>


struct FunctionInfo {
	int pointer;
	std::string signature;
};

class FunctionMap {
private:
	std::vector<FunctionInfo> functions;
public:
	FunctionInfo& getFunction(int index);
	void putFunction(FunctionInfo info);
};

#endif