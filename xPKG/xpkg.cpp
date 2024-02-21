#include "xpkg.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>

#define MAGIC_NUMBER        0 //0x8475

xpkg_symbol_table::xpkg_symbol_table() {
	table = {};
}

void xpkg_symbol_table::insert(xpkg_symbol& _Symbol) {
	table.insert({ _Symbol.name, _Symbol });
}

xpkg_symbol* xpkg_symbol_table::lookup(std::string& _Name, std::vector<std::string> _Params = {}) {
	auto it = table.find(_Name);

	if (it != table.end())
		if (it->second.parameters.size() == _Params.size())
			return &(it->second);
	else
		return nullptr;
}

void xpkg_symbol_table::erase(std::string& _Name) {
	table.erase(_Name);
}