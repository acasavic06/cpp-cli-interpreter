#pragma once
#include <string>
#include <vector>
using namespace std;

class Exception {
public:
	Exception(vector<string>);
	virtual string getMessage() = 0;
protected:
	vector <string> tokens;
};

class FileAlreadyExist : public Exception {
public:
	FileAlreadyExist(vector<string> tokens, string fileName);
	string getMessage() override;
private:
	string fileName;
};

class FileNotFound : public Exception {
public:
	FileNotFound(vector<string> tokens, string fileName);
	string getMessage() override;
private:
	string fileName;
};

class WrongCommandName : public Exception {
public:
	using Exception::Exception;
	string getMessage() override;
};

class WrongNumberOfArgs: public Exception {
public:
	using Exception::Exception;
	string getMessage() override;
};


class DoubleInput : public Exception {
public:
	DoubleInput(vector<string> tokens, string fileName);
	string getMessage() override;
private:
	string fileName;
};

class DoubleOutput: public Exception {
public:
	DoubleOutput(vector<string> tokens, string fileName);
	string getMessage() override;
private:
	string fileName;
};

class WrongOutput: public Exception {
public:
	WrongOutput(vector<string> tokens, string fileName);
	string getMessage() override;
private:
	string fileName;
};

class PipeCmd : public Exception {
public:
	PipeCmd(vector<string>tokens, string message);	
	string getMessage() override;
private:
	string message;
};

class QuotesOnError : public Exception {
public:
	using Exception::Exception;
	string getMessage() override;
};

class InvalidCharError : public Exception {
public:
	InvalidCharError(vector<string>tokens, string message);
	string getMessage() override;
private:
	string line;
};


