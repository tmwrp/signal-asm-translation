#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>


struct Token {
	int coding, line, col;
	std::string strT;
 	Token(int coding, int line, int col, std::string strT) {
		this->coding = coding;
		this->line = line;
		this->col = col;
		this->strT = strT;
	}
};

enum sTypes {dig, let, com, dm, ws};

class Lexer
{
private:
	std::string test;
	const int intB = 501; 
	const int indentB = 1001; 

	void filereader();
	void comreader(int&, int&, std::ifstream&);
	void intreader(int&, int&, int&, std::ifstream&);
	void strreader(int&, int&, int&, std::ifstream&);
	void dmreader(int&, int&, int&, std::ifstream&);

public:
	int idntsize;
	std::vector<Token> tokens;
	std::vector<std::string> errors;
	std::map<std::string, int> identifiers;
	std::map<std::string, int> keywords;
	std::map<std::string, int> integers;
	std::map<int, sTypes> alph;

	Lexer(std::string);
};

