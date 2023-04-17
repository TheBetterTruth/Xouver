#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include "parsing/ast.h"

extern unsigned int ptr;

bool match(const std::string src, const char _char);
bool match(const std::string src, const std::string str);
void write_bytes(const void* val, const size_t size, std::vector<unsigned char>& out);
const Type type_from(const std::unique_ptr<const ASTExpr>& expr);
#endif