#ifndef FUNCTIONMAP_H
#define FUNCTIONMAP_H

#include <string>
#include <vector>
#include <string>

struct FunctionInfo {
	int pointer;
	std::string signature;
};

class FunctionMap {
private:
	std::vector<FunctionInfo> functions;
public:
	FunctionInfo& getFunction(int index);
	FunctionInfo& getFunction(std::string signature);
	void putFunction(FunctionInfo info);
};

#endif