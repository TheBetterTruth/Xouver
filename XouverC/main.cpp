#include <lexing/lexer.h>
#include <lexing/token.h>
#include <parsing/parser.h>
#include <parsing/ast.h>

#include <memory>
#include <cstdio>
#include <fstream>
#include <iostream>
#include "exception.h"
#include "common.h"

#include "codegen/class.h"

unsigned int ptr;

int main() {
	std::string currentFile = "Test.xs";
	std::fstream file(currentFile);

	try {
		std::vector<std::unique_ptr<const Token>> tokens = lex(file);
		std::vector<std::unique_ptr<const ASTExpr>> exprs = parse(&tokens);
		
		std::vector<unsigned char> cls;
		classes.push_back(std::make_unique<Class>("Test", "Xouver.Test", exprs));

		Class& c = *classes[0];
		_currentClass = &c;
		std::vector<unsigned char> content;
		c.gen(content);

		int classCount = 1;
		write_bytes(&classCount, sizeof(classCount), cls);
		int nameLen = c.fullName().length();
		write_bytes(&nameLen, sizeof(nameLen), cls);
		for (unsigned char c : c.fullName())
			cls.push_back(c);

		int scopeSize = c.ptr();
		write_bytes(&scopeSize, sizeof(scopeSize), cls);

		int funcCount = c.funcCount();
		write_bytes(&funcCount, sizeof(funcCount), cls);

		for (int i = 0; i < funcCount; i++) {
			const Function& func = c.getFunc(i);

			int sigLen = func.signature.length();
			std::cout << func.signature << '\n';
			write_bytes(&sigLen, sizeof(sigLen), cls);
			for (unsigned char c : func.signature)
				cls.push_back(c);
			write_bytes(&func.ptr, sizeof(func.ptr), cls);
		}
		const std::vector<Constant>& constants = c.constants();
		int poolSize = constants.size();
		write_bytes(&poolSize, sizeof(poolSize), cls);

		for (auto& con : constants) {
			if (con.type == ConstantType::C_INT) {
				cls.push_back(11);
				write_bytes(&con.value.i, sizeof(con.value.i), cls);
			}
		}

		int endOff = content.size();
		write_bytes(&endOff, sizeof(endOff), cls);

		for (auto it = content.begin(); it != content.end(); it++) {
			cls.push_back(*it);
		}

		std::fstream file("Test.xvr", std::ios::out|std::ios::binary);
		file.write((char*)cls.data(), cls.size());
	}
	catch (BaseException e) {
		std::cout << "Failed to compile " << currentFile << ":\nError on line " << e.line() << ":\n" << e.what() << "\n";
	}

	return 0;
}