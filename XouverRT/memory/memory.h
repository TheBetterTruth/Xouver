#ifndef MEMORY_H
#define MEMORY_H

#include <XNI.h>
#include <object/object.h>
#include <vector>

typedef struct S_MEMBLOCK_T {
	void* ptr;
} MemBlock;

class MemoryManager {
 private:
	using Memory = std::vector<Object>;
	const void* rt;
	Memory youngA{};
	Memory youngB{};
	Memory oldA{};
	Memory oldB{};

	bool isYoungA{false};
	bool isOldA{false};
	uint64_t youngOffset{0};
	uint64_t oldOffset{0};

	Memory& getYoung();

 public:
	MemoryManager(const void* rt);
	Object& allocate(const xClass& classObj);
	~MemoryManager();
};

#endif