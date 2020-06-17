#pragma once
#include <vector>
#include <string>
#include "Lexer.h"
#include "Parser.h"
#include "gen.h"

class output
{
public:
	output() = default;
	output(std::string);
	void lexer(std::vector<std::string>, std::vector<Token>);
	void parser(TreeNode*, Parser);
	void generate(std::string, gen&);
private: 
	std::string name;
	void tree_print(TreeNode*, std::ofstream&);
};

