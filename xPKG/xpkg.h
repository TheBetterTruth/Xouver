#ifndef XPKG_H
#define XPKG_H

#include <stdint.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <streambuf>

class xpkg_bytecode_stream {
private:
	std::streambuf* buffer;
	unsigned int ptr;
public:
	xpkg_bytecode_stream(std::streambuf* _Buffer);
	void write_byte(unsigned char _Byte);
	void write_double(double _Value);
	unsigned char read_byte();
	double read_double();
	unsigned int pointer();
};

struct xpkg_symbol {
	std::string name;
	std::string symbol_type;
	std::string type;
	std::string base_type;					// Only used by classes
	std::vector<std::string> parameters;
	unsigned int line_number;
};

class xpkg_symbol_table {
private:
	std::unordered_map<std::string, xpkg_symbol> table;
public:
	xpkg_symbol_table();
	void insert(xpkg_symbol& _Symbol);
	xpkg_symbol* lookup(std::string& _Name, std::vector<std::string> _Params = {});
	void erase(std::string& _Name);
};


struct xpkg_constant {
	enum constant_type {
		CONST_NUM,
		CONST_BOOL,
		CONST_CHAR,
		CONST_STRING,
		CONST_IDENT
	} type;
	std::string value;
};




#endif