#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

class Command {
public:
	Command(istream* in = &cin, ostream* out = &cout);
	virtual ~Command()=default;
	virtual void execute() = 0;
protected:
	string getInputString() {
		ostringstream ss;
		ss << (*in).rdbuf();
		return ss.str();
	}
	
	//istream& in;
	//ostream& out;
	istream* in;
	ostream* out;
};

// Zero command -> komande bez inputa i opcija
// File command -> komande koje barataju fajlovima
// One command -> komande sa inputom ali bez opcija
// Two command -> komande i sa inputom i sa opcijama
// Three command -> komanda sa inputom, outputom,(opciono) opcijama i vektorom prosledjenih argumenata

class ZeroCommand : public Command {
public:
	using Command::Command;
protected:
	string getTimeAndDate();
};

class OneCommand : public Command {
public:
	using Command::Command;
};

class FileCommand : public Command {
public:
	FileCommand(istream* in, ostream* out, string filename);
	bool fileExists() const {
		ifstream f(filename);
		return f.is_open();
	}
protected:
	string filename;
};

class TwoCommand : public Command {
public:
	TwoCommand(istream* in, ostream* out, vector<string> options);
protected:
	vector<string> options;
	string data;
};

class ThreeCommand : public Command {
public:
	ThreeCommand(istream* in, ostream* out, vector<string> options,vector<string> arguments);
protected:
	vector<string> options;
	vector<string> arguments;
	string data;
};


class TimeCommand : public ZeroCommand {
public:
	using ZeroCommand::ZeroCommand;
	void execute() override;
};

class DateCommand : public ZeroCommand {
public:
	using ZeroCommand::ZeroCommand;
	void execute() override;
};

class EchoCommand : public OneCommand {
public:
	using OneCommand::OneCommand;
	void execute() override;
};

class PromptCommand : public OneCommand {
public:
	using OneCommand::OneCommand;
	void execute() override;
};

class TouchCommand : public FileCommand {
public:
	using FileCommand::FileCommand;
	void execute() override;
};

class TruncateCommand : public FileCommand {
public:
	using FileCommand::FileCommand;
	void execute() override;
};

class RmCommand : public FileCommand {
public:
	using FileCommand::FileCommand;
	void execute() override;
};

class BatchCommand : public FileCommand {
public:
	BatchCommand(istream* in, ostream* out, string filename,vector<string> filenames);
	//using FileCommand::FileCommand;
	void execute() override;
private:
	vector <string> filenames;
};

class HeadCommand : public TwoCommand {
public:
	using TwoCommand::TwoCommand;
	void execute() override;
};

class WCCommand : public TwoCommand {
public:
	using TwoCommand::TwoCommand;
	virtual void execute() override;
private:
	int countWords();
	int countCharacters();
};

class TrCommand : public ThreeCommand {
public:
	using ThreeCommand::ThreeCommand;
	virtual void execute() override;
private:
	string cutString(const string& s) {
		return s.substr(1, s.size() - 2);
	}
};

class PipeCommand : public Command {
public:
	PipeCommand(istream* in,ostream* out,vector<Command*> commands);
	virtual void execute()override;
	~PipeCommand() {
		for (Command* cmd : commands) {
			delete cmd;
		}
	}
	PipeCommand(const PipeCommand&) = delete;
	PipeCommand& operator=(const PipeCommand&) = delete;
private:
	vector <Command*> commands;
};
