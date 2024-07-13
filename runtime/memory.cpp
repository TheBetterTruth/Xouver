#include <vector>
#include "memory.h"

std::vector<mem_block> mem_vec{};

size_t new_block() {
	mem_vec.push_back(mem_block{});
	return mem_vec.size()-1;
}

object_ptr alloc_obj() {
	size_t idx = 0;
	bool found = false;

	if (!mem_vec.empty()) {
		for (size_t i = 0; i < mem_vec.size(); i++) {
			if (mem_vec[i].is_free) {
				idx = i;
				found = true;
			}
		}
	}

	if (!found)
		idx = new_block();
	
	mem_vec[idx].is_free = false;
	mem_vec[idx].object = std::make_shared<object>();
	mem_vec[idx].object->block_index = idx;

	return mem_vec[idx].object;
}

void free_obj(object* _Object) {
	size_t idx = _Object->block_index;

	mem_vec[idx].is_free = true;
	mem_vec[idx].object = {};
}