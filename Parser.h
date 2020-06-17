#pragma once
#include <algorithm>
#include "Lexer.h"



struct TreeNode {
	std::string name = "";
	int coding = -1;
	int line = -1, col = -1;
	int depth = 0; 
	bool is_terminal = false;

	TreeNode* next = nullptr; 
	TreeNode* prev = nullptr;
	TreeNode* down = nullptr;
	TreeNode* up = nullptr;
};

class Parser
{
private:

	Token* nextTk(bool);

	bool program(Token* token, TreeNode* nodeptr);

	bool block(Token* token, TreeNode* nodeptr);

	bool decl(Token* token, TreeNode* nodeptr);

	bool funclist(Token* token, TreeNode* nodeptr);

	bool func(Token* token, TreeNode* nodeptr);

	bool funcidnt(Token* token, TreeNode* nodeptr);

	bool funcchar(Token* token, TreeNode* nodeptr);

	bool uns_int(Token* token, TreeNode* nodeptr);

	bool pridnt(Token* token, TreeNode* nodeptr);

	bool mfdecl(Token* token, TreeNode* node);

	bool constant(Token* token, TreeNode* node);

	TreeNode* create_tree();

	TreeNode* append(TreeNode* current, std::string name, bool is_terminal, int line, int col, int coding, bool next, int depth); 

	std::vector<Token> lextable;
	
	std::string where(const Token*) const;

	Token* ptrTk = nullptr;
	TreeNode* ptrNd;
	TreeNode* ptrNd_;
	TreeNode* ptrNdLst;

	int ident_count;
	const int ident_min = 1001;
	int ident_max;
	int const_count;
	const int const_min = 501;
	int const_max;
	unsigned int i = 0;
	bool first_list = true;

	int list = 15;

public:
	
	bool status = true;
	std::string errorMessage;
	TreeNode* tree; 

	Parser(const std::vector<Token>& table, std::map<std::string, int>& identifiers, std::map<std::string, int>& integers, std::map<std::string, int>& keywords);

	bool beg();
};
