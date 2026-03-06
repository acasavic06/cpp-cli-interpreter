#pragma once
#include <string>
#include <iostream>
using namespace std;

class Reader {
public:
	Reader(istream* input = &cin);
	virtual ~Reader() {};

	string getLine();
	bool isEof();

private:
	istream* input;
};
