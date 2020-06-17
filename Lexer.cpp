#include "Lexer.h"

void Lexer::filereader() {
	int temp;
	int cline = 1, ccol = 0;
	std::string word;
	std::ifstream testfile(test);
	if (testfile.is_open()) {
		temp = testfile.get();
		while (temp != EOF) {
			const auto key = alph.find(temp);
			if (key == alph.cend()) {
				std::string s = "Lexer Error: Unknown symbol \'";
				s += char(temp);
				++ccol;
				errors.push_back(s + "\' at(" + std::to_string(cline) + ';' + std::to_string(ccol) + ')');
				temp = testfile.get();
				continue;
			}
			else {
				int RES = key->second;
				switch (RES) {
				case 0:
					intreader(temp, cline, ccol, testfile);
					continue;
				case 1:
					strreader(temp, cline, ccol, testfile);
					continue;
				case 2:
					temp = testfile.get();
					if (temp == 42) {
						ccol += 2;
						comreader(cline, ccol, testfile);
					}
					else {
						++ccol;
						errors.push_back("Lexer Error: Wrong use of '('; Opened at(" + std::to_string(cline) + ';' + std::to_string(ccol) + ')');
					}
					temp = testfile.get();
					continue;
				case 3:
					dmreader(temp, cline, ccol, testfile);
					continue;
				case 4:
					if (temp == 10)
					{
						++cline;
						ccol = 0;
					}
					else if (temp == 9) {
						ccol += 4;
					}
					else
					{
						++ccol;
					}
					temp = testfile.get();
					break;
				default:
					std::cout << "Project error: Something went wrong at Lexer::filereader()" << std::endl;
				}
			}
		}
		testfile.close();
	}
	else {
		std::cout << "Project error: " << test << " doesn't exist or can not be opened" << std::endl;
	}
}



void Lexer::dmreader(int& Beg, int& line, int& col, std::ifstream& testfile) {
	std::string tmp;
	tmp.push_back((char)Beg);
	char dm1 = tmp[0];
	++col;
	tokens.push_back(Token((int)dm1, line, col, tmp));
	tmp.clear();

	Beg = testfile.get();
}

void Lexer::comreader(int& line, int& col, std::ifstream& testfile) {
	int openedatline = line;
	int openedatcol = col - 1;
	int temp = testfile.get();
	bool commentisclosed = false;
	while (temp != EOF) {
		if (temp == 32) {
			++col;
		}
		else if (temp == 42) {
			while (temp == 42) {
				temp = testfile.get();
				++col;
			}
			if (temp == 41) {
				commentisclosed = true;
				break;
			}
		}
		else if (temp == 10) {
			++line;
			col = 1;
		}
		else {
			++col;
		}
		temp = testfile.get();
	}
	if (!commentisclosed) {
		errors.push_back("Lexer Error: Comment is not closed; Opened at(" + std::to_string(openedatline) + ';' + std::to_string(openedatcol) + ')');
	}
}

void Lexer::intreader(int& Beg, int& line, int& col, std::ifstream& testfile) {
	std::string result;
	bool error = false;
	int err_col, i = 0;
	while (isdigit((char)Beg) || isalpha((char)Beg)) {
		result += Beg;
		++i;
		if (isalpha((char)Beg)) {
			err_col = col + i;
			error = true;
		}
		Beg = testfile.get();
	}
	++col;
	if (!error) {
		const auto s = integers.find(result);
		if (s == integers.cend()) {
			integers.insert(std::pair<std::string, int>(result, intB + integers.size()));
			tokens.push_back(Token(integers[result], line, col, result));
		}
		else {
			tokens.push_back(Token(integers[result], line, col, result));
		}
	}
	else {
		errors.push_back("Lexer Error: Letter is a part of integer at(" + std::to_string(line) + ';' + std::to_string(err_col) + ')');
	}
	col += result.length() - 1;
	result.clear();
}

void Lexer::strreader(int& Beg, int& line, int& col, std::ifstream& testfile) {
	std::string result;
	++col;
	result += Beg;
	Beg = testfile.get();
	while (isdigit((char)Beg) || isalpha((char)Beg)) {
		result += Beg;
		Beg = testfile.get();
	}
	const auto s = identifiers.find(result);
	const auto m = keywords.find(result);
	if (m != keywords.cend()) {
		tokens.push_back(Token(keywords[result], line, col, result));
	}
	else {
		identifiers.insert(std::pair<std::string, int>(result, indentB + identifiers.size()));
		tokens.push_back(Token(identifiers[result], line, col, result));
	}
	col += (result.length() - 1);
	result.clear();
}

Lexer::Lexer(std::string test) {
	tokens = std::vector<Token>();
	errors = std::vector<std::string>();
	keywords = std::map<std::string, int>{
		{"PROGRAM", 401},
		{"BEGIN", 402},
		{"END", 403},
		{"DEFFUNC", 404},
	};
	identifiers = std::map<std::string, int>();
	integers = std::map<std::string, int>();
	alph = std::map<int, sTypes>();
	for (int i = 'A'; i <= 'Z'; ++i) {
		alph.insert(std::pair<int, sTypes>(i, sTypes::let));
	}
	for (int i = 'a'; i <= 'z'; ++i) {
		alph.insert(std::pair<int, sTypes>(i, sTypes::let));
	}
	for (int i = '0'; i <= '9'; ++i) {
		alph.insert(std::pair<int, sTypes>(i, sTypes::dig));
	}


	for (int i = 8; i <= 13; ++i) {
		alph.insert(std::pair<int, sTypes>(i, sTypes::ws));
	}
	alph.insert(std::pair<int, sTypes>(32, sTypes::ws));
	alph.insert(std::pair<int, sTypes>('.', sTypes::dm));
	alph.insert(std::pair<int, sTypes>('=', sTypes::dm));
	alph.insert(std::pair<int, sTypes>(';', sTypes::dm));
	alph.insert(std::pair<int, sTypes>('\\', sTypes::dm));
	alph.insert(std::pair<int, sTypes>(',', sTypes::dm));
	alph.insert(std::pair<int, sTypes>('*', sTypes::dm));
	alph.insert(std::pair<int, sTypes>('(', sTypes::com));
	alph.insert(std::pair<int, sTypes>(')', sTypes::dm));
	this->test = test;
	filereader();
	this->idntsize = identifiers.size();
}
