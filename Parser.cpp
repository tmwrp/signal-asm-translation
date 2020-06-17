#include "Parser.h"

#define N TreeNode
#define NEXT n->next
#define DOWN n->down
#define GET token = nextTk();

Parser::Parser(const std::vector<Token>& table, std::map<std::string, int>& identifiers, std::map<std::string, int>& integers, std::map<std::string, int>& keywords) {
	lextable = table;
	ident_count = identifiers.size();
	const_count = integers.size();
	ident_max = ident_count + ident_min;
	const_max = const_count + const_min;
}

std::string Parser::where(const Token* t) const {
	return  "at line : " + std::to_string(t->line) + ", col : " + std::to_string(t->col);
}

TreeNode* Parser::create_tree() {
	TreeNode* root = new TreeNode;
	root->name = "<signal-program>";
	return root;
}

TreeNode* Parser::append(TreeNode* current, std::string name, bool is_terminal, int line, int col, int coding, bool next, int depth) {
	TreeNode* node = new TreeNode;
	node->depth = depth;
	node->name = name;
	node->col = col;
	node->line = line;
	node->is_terminal = is_terminal;
	node->coding = coding;

	if (next) {
		node->prev = current;
	}
	else {
		node->up = current;
	}

	return node;
}

Token* Parser::nextTk(bool check=false) {
	if (++i < lextable.size()) {
		return &lextable[i];
	}
	if (!check) {
		std::cerr << "Unexpected end of file." << std::endl;
		exit(-1);
	}
	else {
		return nullptr;
	}
}

bool Parser::beg() {
	tree = create_tree();
	return status = program(&lextable[0], tree);
}


bool Parser::program(Token* token, TreeNode* node) {
	N* n = node;
	n = DOWN = append(n, "<program>", false, -1, -1, -1, false, 3);
	if (token->strT == "PROGRAM") {
		n = DOWN = append(n, token->strT, true, token->line, token->col, token->coding, false, 6);
		if (pridnt(token, n)) {
			n = ptrNd;
			GET
				if (token->strT == ";") {
					n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, 6);
					if (block(token, n)) {
						n = ptrNd_;
						GET
							if (token->strT == ".") {
								n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, 6);
								token = nextTk(true);
								if (token != nullptr) {
									errorMessage = "Parser Error: Code after the end of the block is forbidden.";
									return false;
								}
								return true;
							}
							else {
								errorMessage = "Parser Error: Delimiter \".\" expected but " + token->strT + " found " + where(token);
								return false;
							}
					}
					else {
						return false;
					}
				}
				else {
					errorMessage = "Parser Error: Delimiter \";\" expected but " + token->strT + " found " + where(token);
					return false;
				}
		}
		else {
			return false;
		}
	}
	else {
		errorMessage = "Parser Error: Keyword \"PROGRAM\" expected but " + token->strT + " found " + where(token);
		return false;
	}
}

bool Parser::block(Token* token, TreeNode* node) {
	N* n = node;
	n = NEXT = append(n, "<block>", false, -1, -1, -1, true, 6);
	ptrNd_ = n;
	if (decl(token, n)) {
		n = DOWN;

			token = ptrTk;
	
			if (token->strT == "BEGIN") {
				n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, 9);
				GET
					if(token->strT == "END"){
						n = NEXT = append(n, "<statements-list>", false, -1, -1, -1, true, 9);
						DOWN = append(n, "<empty>", false, -1, -1, -1, false, 12);
						n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, 9);
					}
					else {
						errorMessage = "Parser Error: Keyword \"END\" expected but " + token->strT + " found " + where(token);
						return false;
					}
			}
			else {
				errorMessage = "Parser Error: Keyword \"BEGIN\" expected but " + token->strT + " found " + where(token);
				return false;
			}
	}
	else {
		return false;
	}

	return true;
}

bool Parser::decl(Token* token, TreeNode* node) {
	N* n = node;
	n = DOWN = append(n, "<declarations>", false, -1, -1, -1, false, 9);
	n = DOWN = append(n, "<math-function-declarations>", false, -1, -1, -1, false, 12);
	if (mfdecl(token, n)) {
		return true;
	}
	else {
		DOWN = append(n, "<empty>", false, -1, -1, -1, false, 15);
		return true;
	}
}

bool Parser::mfdecl(Token* token, TreeNode* node) {
	N* n = node;
	GET
		if (token->strT == "DEFFUNC") {
			n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, 15);
			if (funclist(token, n)) {
				return true;
			}
			else {
				n = ptrNdLst;
				DOWN = append(n, "<empty>", false, -1, -1, -1, false, list + 3);
				return true;
			}
		}
		else {
			ptrTk = token;
			return false;
		}
}

bool Parser::funclist(Token* token, TreeNode* node) {
	N* n = node;
	if (first_list) {
		first_list = false;
		n = NEXT = append(n, "<function-list>", false, -1, -1, -1, true, list);
		ptrNdLst = n;
	}
	else {
		n = ptrNdLst;
		n = NEXT = append(n, "<function-list>", false, -1, -1, -1, true, list);
		ptrNdLst = n;
	}
	if (func(token, n)) {
		list += 3;
		if (funclist(token, n)) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

bool Parser::func(Token* token, TreeNode* node) {
	N* n = node;
	if (funcidnt(token, n)) {
		GET
			if (token->strT == "=") {
				n = NEXT;
				n = DOWN;
				n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, list + 6);
				if (constant(token, n)) {
					n = NEXT;
					if (funcchar(token, n)) {
						return true;
					}
					else {
						return false;
					}
				}
				else {
					return false;
				}
			}
			else {
				errorMessage = "Parser Error: Delimiter \"=\" expected but " + token->strT + " found " + where(token);
				return false;
			}
	}
	else {
		return false;
	}
}
//
bool Parser::funcidnt(Token* token, TreeNode* node){
	N* n = node;
	GET

	if (((token->coding) <= ident_max) && ((token->coding) >= ident_min)) {
		n = NEXT = append(n, "<function>", false, -1, -1, -1, true, list + 3);
		ptrNdLst = n;
		n = DOWN = append(n, "<function-identifier>", false, -1, -1, -1, false, list + 6);
		n = DOWN = append(n, "<identifier>", false, -1, -1, -1, false, list + 9);
		DOWN = append(n, token->strT, true, token->line, token->col, token->coding, false, list + 12);
		return true;
	}
	else {
			ptrTk = token;
			return false;
	}
}

bool Parser::funcchar(Token* token, TreeNode* node) {
	N* n = node;
	n = NEXT = append(n, "<function-characteristic>", false, -1, -1, -1, true, list + 6);
	GET
		if (token->strT == "\\") {
			n = DOWN = append(n, token->strT, true, token->line, token->col, token->coding, false , list + 9);
			if (uns_int(token, n)) {
				n = NEXT;
				GET
					if (token->strT == ",") {
						n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, list + 9);
						if (uns_int(token, n)) {
							n = NEXT;
							GET
								if (token->strT == ";") {
									n = NEXT = append(n, token->strT, true, token->line, token->col, token->coding, true, list + 9);
									return true;
								}
								else {
									errorMessage = "Parser Error: Delimiter \";\" expected but " + token->strT + " found " + where(token);
									return false;
								}
						}
						else {
							return false;
						}
					}
					else {
						errorMessage = "Parser Error: Delimiter \",\" expected but " + token->strT + " found " + where(token);
						return false;
					}
			}
			else {
				return false;
			}
		}
		else {
			errorMessage = "Parser Error: Delimiter \"\\\" expected but " + token->strT + " found " + where(token);
			return false;
		}
}

bool Parser::uns_int(Token* token, TreeNode* node) {
	N* n = node;
	GET
	n = NEXT = append(n, "<unsigned-integer>", false, -1, -1, -1, true, list + 9);
	if (((token->coding) <= const_max) && ((token->coding) >= const_min)) {
		DOWN = append(n, token->strT, true, token->line, token->col, token->coding, false, list + 12);
		return true;
	}
	errorMessage = "Parser Error: unsigned integer was expected " + where(token);
	return false;
}

bool Parser::constant(Token* token, TreeNode* node) {
	N* n = node;
	GET
	n = NEXT = append(n, "<constant>", false, -1, -1, -1, true, list + 6);
	
	
	if (((token->coding) <= const_max) && ((token->coding) >= const_min)) {
		n = DOWN = append(n, "<unsigned-integer>", false, -1, -1, -1, false, list + 9);
		n = DOWN = append(n, token->strT, true, token->line, token->col, token->coding, false, list + 12);
		return true;
	}
	errorMessage = "Parser error: unsigned integer was expected" + where(token);
	return false;
	
}

bool Parser::pridnt(Token* token, TreeNode* node) {
	N* n = node;
	GET
	
	n = NEXT = append(n, "<procedure-identifier>", false, -1, -1, -1, true, 6);
	ptrNd = n;
	if (((token->coding) <= ident_max) && ((token->coding) >= ident_min)) {
		n = DOWN = append(n, "<identifier>", false, -1, -1, -1, false, 9);
		DOWN = append(n, token->strT, true, token->line, token->col, token->coding, false, 12);
		return true;
	}
	else {
		errorMessage = "Parser Error: Wrong procedure identifier or procedure identifier is missing " + where(token);
		return false;
	}
}
