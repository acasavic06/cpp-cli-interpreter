#include "Reader.h"
#include <string>
#include <limits>

using namespace std;

Reader::Reader(istream* input)
	: input(input) {
}

string Reader::getLine() {
	string line;
	getline(*input, line);
	if (line.size() > 512) {
		line = line.substr(0, 512);
		input->ignore(numeric_limits<streamsize>::max(), '\n');
	}
	return line;
}

bool Reader::isEof() {
	if (input->eof()) {
		input->clear();
		return true;
	}
	return false;
}
