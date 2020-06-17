#include <iomanip>
#include "gen.h"


gen::gen(TreeNode* Tree) {
	this->Tree = Tree;
	this->List = nullptr;
	this->End = nullptr;
	this->count = 0;
}


void gen::Scanner(TreeNode* Tree){

	while (Tree) {
		if (!Tree->is_terminal) {
			if (Tree->name == "<function>") {
				
				++count;
				FuncScanner(Tree);

			}
		}

		if (Tree->down != nullptr)
			Scanner(Tree->down);
		Tree = Tree->next;
	}

}



void gen::init() {

	Scanner(Tree);

	if (ListScanner()) {
		std::cout << "Compiled successfully..." << std::endl;
	}
	else {
		std::cout << "Semantic errors found..." << std::endl;
	}
	
}

void gen::FuncScanner(TreeNode* Fnc) {

	int size = atoi(Fnc->down->next->next->next->down->next->next->next->down->name.c_str());
	int value = atoi(Fnc->down->next->next->down->down->name.c_str());
	int limit = atoi(Fnc->down->next->next->next->down->next->down->name.c_str());
	int const_col = Fnc->down->next->next->down->down->col;
	int const_line = Fnc->down->next->next->down->down->line;
	int uint_col = Fnc->down->next->next->next->down->next->next->next->down->col;
	int uint_line = Fnc->down->next->next->next->down->next->next->next->down->line;
	int top_col = Fnc->down->next->next->next->down->next->down->col;
	int top_line = Fnc->down->next->next->next->down->next->down->line;
	

	if (List != nullptr) {

		End->next = Append(End, count, Fnc->down->down->down->name, limit, value, size, Fnc->down->down->down->line, Fnc->down->down->down->col, const_col, const_line, uint_col, uint_line, top_col, top_line);
		End = End->next;

	}else {

		List = Append(nullptr, count, Fnc->down->down->down->name, limit, value, size, Fnc->down->down->down->line, Fnc->down->down->down->col, const_col, const_line, uint_col, uint_line, top_col, top_line);
		End = List;

	}
	
}

Deffunc* gen::Append(Deffunc* prev, int count, std::string name, int limit, int value, int size, int line, int col, int cc, int cl, int uc, int ul, int tc, int tl) {

	Deffunc* func = new Deffunc;
	func->prev = prev;
	func->next = nullptr;
	func->num = count;
	func->name = name;
	func->value = value;
	func->limit = limit;
	func->size = size;
	func->line = line;
	func->col = col;
	func->cc = cc;
	func->cl = cl;
	func->ul = ul;
	func->uc = uc;
	func->tc = tc;
	func->tl = tl;

	return func;
}


bool gen::ListScanner() {

	bool flag = true;
	Deffunc* tmp = List;

	while (tmp != nullptr) {

		Deffunc* prev = tmp->prev;

		if (Tree->down->down->next->down->down->name == tmp->name) {

			errs.push_back("Semantic Error: at(" + std::to_string(tmp->line) + ";" + std::to_string(tmp->col) + ") Function name " + tmp->name + " copies program name. Original declaration at(" +
				std::to_string(Tree->down->down->next->down->down->line) + ";" + std::to_string(Tree->down->down->next->down->down->col) + ")");
			if (err_line == -1) err_line = tmp->line;
			flag = false;

		}

		while (prev != nullptr) {

			if (prev->name == tmp->name) {

				errs.push_back("Semantic Error: at(" + std::to_string(tmp->line) + ";" + std::to_string(tmp->col) + ") Function " + tmp->name + " duplicates another function\'s name. Original declaration at(" +
					std::to_string(prev->line) + ";" + std::to_string(prev->col) + ")");
				flag = false;
				if (err_line == -1) err_line = tmp->line;
				break;

			}

			prev = prev->prev;
		}

		if (tmp->limit > 65535) {

			errs.push_back("Semantic Error: at(" + std::to_string(tmp->tl) + ";" + std::to_string(tmp->tc) + ") Incorrect top limit for [" + tmp->name + "] array elements\' value. (Out of bounds)");
			flag = false;
			if (err_line == -1) err_line = tmp->line;

		}

		if (tmp->size > 65535) {

			errs.push_back("Semantic Error: at(" + std::to_string(tmp->ul) + ";" + std::to_string(tmp->uc) + ") Incorrect size of the [" + tmp->name + "] array. (Out of bounds)");
			flag = false;
			if (err_line == -1) err_line = tmp->line;

		}

		if (tmp->value > 65535) {

			errs.push_back("Semantic Error: at(" + std::to_string(tmp->cl) + ";" + std::to_string(tmp->cc) + ") Incorrect init value for [" + tmp->name + "] array elements. (Out of bounds)");
			flag = false;
			if (err_line == -1) err_line = tmp->line;

		}

		tmp = tmp->next;
	}

	return flag;
}