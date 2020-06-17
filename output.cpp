#include <fstream>
#include <iomanip>
#include "output.h"	
#include "Parser.h"	
#include "gen.h"

void output::lexer(std::vector<std::string> errs, std::vector<Token> tkns) {
	std::ofstream tokens(name + "/table_of_tokens.txt");
	tokens << "Lexical errors : " << errs.size() << std::endl;
	for (auto i : errs)
		tokens << i << std::endl;
	tokens << std::endl;

	tokens << "Line:" << "\t" << "Col:" << "\t" << "Token:" << "\t\t" << "Coding:" << std::endl << std::endl;
	for (auto i : tkns)
		tokens << i.line << "\t" << i.col << "\t" << i.strT << "\t\t" << i.coding << std::endl;
	tokens << std::endl;
	tokens.close();
	
}

output::output(std::string pathName) {
	name = pathName;
}

void output::parser(TreeNode* tree, Parser p) {
	std::ofstream parserRes(name + "/tree.txt");
	tree_print(tree, parserRes);
	if (p.errorMessage.length() > 0) {
		parserRes << std::endl << p.errorMessage;
	}
	parserRes.close();
}

void output::tree_print(TreeNode* root, std::ofstream& out) {

	while (root != nullptr) {
		for (int s = 0; s < root->depth; ++s) {
			out << '.';
		}
		if (root->is_terminal) {
			out << root->coding << " " << root->name << std::endl;
		}
		else {
			out << root->name << std::endl;
		}
		if (root->down != nullptr)
			tree_print(root->down, out);
		root = root->next;
	}
}


void output::generate(std::string path, gen& obj) {

	std::ofstream codegen(path);
	if (codegen.is_open()) {

		Deffunc* tmp = obj.List;

		std::cout << "Generating Code...\n" << std::endl;

		codegen << ".i386\n\n" << std::endl;

		codegen << ".data" << std::endl;
		while (tmp != nullptr) {

			if (obj.err_line == -1 || tmp->line < obj.err_line) {
				codegen << tmp->name << " DW " << tmp->size << " DUP (" << tmp->value <<  ")" << std::endl;
				tmp = tmp->next;
			}
			else {
				break;
			}

		}

		codegen << "\n\n.code" << std::endl;

		codegen << "NOP\n" << std::endl;


		if (obj.errs.size() == 0) {
			codegen << "MOV AX, 4с00h" << std::endl << "INT 21h\n\n" << std::endl;
			codegen << ";STATUS OK";
		}
		else {
			for (auto x : obj.errs) {
				codegen << ";" << x << std::endl;
			}
			codegen << "\nMOV AX, 4с00h" << std::endl << "INT 21h\n\n" << std::endl;
			codegen << "\n;STATUS FAILED";
		}
	}
	codegen.close();
}
