#include <iostream>
#include <string>
#include "Lexer.h"
#include "Parser.h"
#include "output.h"
#include "gen.h"


int main(int argc, char** argv)
{
	std::string test;
	if (argv[1] != NULL) {
		test = argv[1];
	}
	else {
		test = "tests/test01";
	}
	Lexer lex(test + "/input.sig");
	output res(test);
	res.lexer(lex.errors, lex.tokens);
	if (lex.errors.size() == 0) {
		Parser ps(lex.tokens, lex.identifiers, lex.integers, lex.keywords);
		ps.beg();
		res.parser(ps.tree, ps);
		if (ps.status) {
			gen code(ps.tree);
			code.init();
			res.generate(test + "/generated.asm", code);

			std::cout << "Translator finished its work" << std::endl;
		}
		else {
			std::cerr << "Parser error; check " << test << "/tree.txt out" << std::endl;
		}
	}
	else {
		std::cerr <<"Lexer error; check " << test << "/table_of_tokens.txt out" << std::endl;
	}
	return 0;
}