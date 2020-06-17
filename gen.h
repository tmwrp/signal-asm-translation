#pragma once
#include "Parser.h"
#include <vector>

struct Deffunc
{

	int num = -1;
	std::string name = "";
	int size = -1;
	int value = -1;
	int limit = -1;
	int line = -1;
	int col = -1;
	int cc = -1;
	int cl = -1;
	int ul = -1;
	int uc = -1;
	int tc = -1;
	int tl = -1;
	Deffunc* next = nullptr;
	Deffunc* prev = nullptr;
};

class gen
{

public:

	int err_line = -1;
	gen(TreeNode*);
	void init();
	Deffunc* List;
	std::vector<std::string> errs;

private:

	int count;
	
	TreeNode* Tree;

	Deffunc* End;

	Deffunc* Append(Deffunc*, int, std::string, int, int, int, int, int, int, int, int, int, int, int);

	void Scanner(TreeNode*);

	void FuncScanner(TreeNode*);

	bool ListScanner();

};

