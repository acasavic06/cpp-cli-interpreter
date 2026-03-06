#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "Commands.h"

class Parser {
public:
	static Parser* getInstance() {
		if (instance == nullptr)
		{
			instance = new Parser();
		}
		return instance;
	}

	//Command* parseCommand(string line);
	Command* parseCommand(string line, istream* in = &cin, ostream* out = &cout, ostream* outError = &cout,bool isBatch = false);
	Command* parsePipeCommand(string line, istream* in = &cin, ostream* out = &cout, ostream* outError = &cout);

	void printArgument() const {
		cout << argument << ' ';
	}

private:
	static Parser* instance;
	Parser();
	vector<string> tokenize(string line);
	vector<string> tokenizePipeCommand(string line);
	
	Command* parseZeroComm(vector<string> tokens);
	Command* parseOneComm(vector<string> tokens);
	Command* parseTwoComm(vector<string> tokens);
	Command* parseThreeComm(vector<string> tokens);
	Command* parseFileComm(vector<string> tokens);

	void createStreams(vector <string> tokens);
	void createOutputStream(string input);
	vector<string> createArguments(vector<string>& tokens);


	//istream* createInputStream(vector <string> tokens, string input);
	//ostream* createOutputStream(string input);

	bool isString(const string& s) {
		return s.size() >= 2 && s.front() == '"' && s.back() == '"';
	}

	int indexPipe(const string s) {
		bool quotesOn = false;
		for (int i = 0; i < s.size(); i++) {
			if (s[i] == '"')quotesOn = !quotesOn;
			if (s[i] == '|') {
				if (quotesOn==false)return i;
			}
		}
		return -1;
	}
	/*bool containsPipe(const string s) {
		for (char c : s) {
			if (c == '|') {
				return true;
			}
		}
		return false;
	}*/

	string cutString(const string& s) {
		return s.substr(1, s.size() - 2);
	}

	string removeEmptySpace(const string s) {
		int i = 0;
		while (i < s.size() && (s[i] == ' ' || s[i]== '\t')) {
			i++;
		}
		return s.substr(i);
	}

	void checkInputError(string const input);

	void checkPipeErrors(vector<string>);
	void checkEmptyPipeCmd(vector<string>);


	vector<string> dodela(vector<string> tokens);


	map<string, Command* (Parser::*)(vector<string>) > mapa;
	istream* in = &cin;
	ostream* out = &cout;
	bool inBatch = false;

	string argument;
};
