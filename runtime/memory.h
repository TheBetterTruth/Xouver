#ifndef MEMORY_H
#define MEMORY_H

#include <string>
#include <variant>
#include <cstdlib>
#include <memory>
#include "xi.h"

struct object;

typedef std::shared_ptr<object> object_ptr;
typedef std::variant<std::monostate, xchar, xshort, xint, xfloat, xlong, xdouble, std::string, object_ptr*> value_t;

object_ptr alloc_obj();
void free_obj(object* _Object);

struct object {
	size_t block_index;
	value_t data;

	object() : block_index(0) {
		data = std::monostate{};
	}

	~object() {
		if (std::holds_alternative<xchar>(data))
			delete std::get<xchar>(data);
		else if (std::holds_alternative<xshort>(data))
			delete std::get<xshort>(data);
		else if (std::holds_alternative<xint>(data))
			delete std::get<xint>(data);
		else if (std::holds_alternative<xfloat>(data))
			delete std::get<xfloat>(data);
		else if (std::holds_alternative<xlong>(data))
			delete std::get<xlong>(data);
		else if (std::holds_alternative<xdouble>(data))
			delete std::get<xdouble>(data);
		else if (std::holds_alternative<object_ptr*>(data)) {
			delete[] std::get<object_ptr*>(data);
		}

		free_obj(this);
	}
};

struct mem_block {
	bool is_free = true;
	object_ptr object;
};

#endif