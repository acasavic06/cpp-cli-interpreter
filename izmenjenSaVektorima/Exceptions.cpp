#include "Exceptions.h"
#include <sstream>
#include <set>

using namespace std;

Exception::Exception(vector<string> tokens)
	: tokens(tokens) {
}

FileAlreadyExist::FileAlreadyExist(vector<string> tokens, string fileName)
	: Exception(tokens), fileName(fileName) {}

FileNotFound::FileNotFound(vector<string> tokens, string fileName)
	: Exception(tokens), fileName(fileName) {}

DoubleInput::DoubleInput(vector<string> tokens, string fileName)
	: Exception(tokens), fileName(fileName) {}

DoubleOutput::DoubleOutput(vector<string> tokens, string fileName)
	: Exception(tokens), fileName(fileName) {}

WrongOutput::WrongOutput(vector<string> tokens, string fileName)
	: Exception(tokens), fileName(fileName) {}

PipeCmd::PipeCmd(vector<string> tokens, string message)
	: Exception(tokens), message(message){}

InvalidCharError::InvalidCharError(vector<string> tokens, string line)
	: Exception(tokens),line(line){}


string FileAlreadyExist::getMessage() {
	return "File " + fileName + " already exist\n";
}

string FileNotFound::getMessage() {
	return "File " + fileName + " not found\n";
}

string WrongCommandName::getMessage() {
	return "This command " + tokens[0] + " doesn't exist\n";
}

string WrongNumberOfArgs::getMessage() {
	stringstream ss;
	ss << "Wrong number of arguments \n";
	return ss.str();
}

string DoubleInput::getMessage()
{
	return "There have been double input in one command\n";
}

string DoubleOutput::getMessage()
{
	return "There have been double output in one command\n";
}

string WrongOutput::getMessage()
{
	return "Izlaz koji ste naveli ne postoji - " + fileName + "\n";
}

string PipeCmd::getMessage()
{
	return message;
}

string QuotesOnError::getMessage()
{
	return "Navodnici nisu zatvoreni \n";
}

string InvalidCharError::getMessage()
{
	set <char> zabranjeniKarakteri = { '$','%','&','@','+','*','?'};
	string errorLine = "";
	bool quotesOn = false;

	for (char c : line) {
		if (c == '"') {
			quotesOn = !quotesOn;
			errorLine += ' ';
		}
		else if (!quotesOn && zabranjeniKarakteri.count(c)) {
			errorLine += '^';
		}
		else {
			errorLine += ' ';
		}
	}
	
	return "Greska - neocekivani karakteri\n" + line + "\n" + errorLine + "\n";
}

