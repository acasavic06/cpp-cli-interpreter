#include "Commands.h"

#include "Exceptions.h"
#include "Parser.h"
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <filesystem>

using namespace std;

Command::Command(istream* in, ostream* out) : in(in), out(out) {}

/*Command::~Command() {
    ofstream* fout = dynamic_cast<ofstream*>(out);
    if (fout) {
        fout->close();
    }
    //if (out != &cout)out->clear();
}*/

FileCommand::FileCommand(istream* in, ostream* out, string filename) : Command(in, out), filename(filename) {}

TwoCommand::TwoCommand(istream* in, ostream* out, vector<string > options) : Command(in, out), options(options){}

ThreeCommand::ThreeCommand(istream* in, ostream* out, vector<string> options, vector<string> arguments) 
    : Command(in, out), options(options), arguments(arguments) {}

BatchCommand::BatchCommand(istream* in, ostream* out, string filename, vector<string> filenames) 
    : FileCommand(in,out,filename), filenames(filenames){}

PipeCommand::PipeCommand(istream* in, ostream* out, vector<Command*> commands) : Command(in,out) , commands(commands) {}


string ZeroCommand::getTimeAndDate() {
    time_t vreme = time(nullptr);
    char buffer[26];
    ctime_s(buffer, sizeof(buffer), &vreme);
    return string(buffer);
}

void TimeCommand::execute() {
    string s = getTimeAndDate();
    *out << s.substr(11, 8) << endl;
}

void DateCommand::execute() {
    string s = getTimeAndDate();
    *out << s.substr(4, 7) << s.substr(20, 4) << endl;
}

void TouchCommand::execute() {
    if (fileExists()) {
        throw FileAlreadyExist({}, filename);
    }
    ofstream file (filename);
}

void TruncateCommand::execute()
{
    if (!fileExists()) {
        throw FileNotFound({}, filename);
    }

    ofstream file(filename);
}

void RmCommand::execute()
{
    if (!fileExists()) {
        throw FileNotFound({}, filename);
    }

    std::filesystem::remove(filename);

/*
    try {
        std::filesystem::remove(filename);
    }
    catch (const std::filesystem::filesystem_error& e) {
        throw std::runtime_error(e.what());
    }*/

}

void BatchCommand::execute()
{
    if (!fileExists()) {
        throw FileNotFound({}, filename);
    }
    ifstream file(filename);

    string line;
    while (getline(file, line)) {
        if (line.empty())continue;
    
        try {
            Command* cmd = Parser::getInstance()->parseCommand(line,this->in,this->out,this->out,true);
            //Treba jos da dodamo output fajl za error, gresku

            if (cmd) {
                in->clear();
                cmd->execute();
                delete cmd;
            }
        }
        catch (WrongCommandName e) {
            cout << e.getMessage();
        }
        catch (FileAlreadyExist e) {
            cout << e.getMessage();
        }
        catch (FileNotFound e) {
            cout << e.getMessage();
        }
        catch (DoubleInput e) {
            cout << e.getMessage();
        }
        catch (DoubleOutput e) {
            cout << e.getMessage();
        }
        catch (PipeCmd e) {
            cout << e.getMessage();
        }
        catch (QuotesOnError e) {
            cout << e.getMessage();
        }
        catch (InvalidCharError e) {
            cout << e.getMessage();
        }
        catch (exception e) {
            cout << "Error in batch line: " << e.what() << endl;
        }
    }
    file.close();
}

void EchoCommand::execute() {
    string text, line;
    while (getline(*in, line)) {
        text += line + "\n";
    }
    *out << text;
    out->flush();
}

void HeadCommand::execute()
{
    string text, line;
    int count = 0,maxN=0;

    if (options.empty() || options[0].substr(0, 2) != "-n") {
        *out << "Greska prilikom unosenja opcije kod funkcije head" << endl;
        return;
    }
    maxN = stoi(options[0].substr(2));
    
    while (getline(*in, line)) {
        if (count++<maxN) text += line + '\n';
    }

/* ako treba da cim iskucamo pet redova prekine progrma
    while (getline(*in, line) && count++ < maxN) {
        text += line + '\n';
    }*/

    /*Poslednjij n redova
    vector<string> r;
    while (getline(*in, line)) {
        r.push_back(line);
    }

    int broj = r.size();

    if (broj < maxN) {
        for (int i = 0; i < broj; i++) {
            text += r[i] + "\n";
        }
    }
    else {
        for (int i = broj-maxN; i < broj; i++) {
            text += r[i] + "\n";
        }
    }*/

    *out << text;
}

void WCCommand::execute() {
    data = getInputString();
    if (options.empty()) {
        throw PipeCmd(vector<string>{}, "wc komanda mora da ima opciju -w ili -c\n");
    }

    if (options[0] == "-w") {
        *out << countWords() << endl;
    }
    else if (options[0] == "-c") {
        *out << countCharacters() << endl;
    }
    else {
        *out << "Greska prilikom unosenja opcije kod funkcije wc" << endl;
    }
}

int WCCommand::countWords() {
    int count = 0;
    bool increase = false;

    for (char c : data) {
        if (isspace(static_cast<unsigned char>(c))) {
            increase = false;
        }
        else {
            if (!increase) {
                count++;
                increase = true;
            }
        }
    }
    return count;
}

int WCCommand::countCharacters() {
    return data.size();
}

void PromptCommand::execute(){}

void TrCommand::execute()
{
    if (options.empty()) { throw PipeCmd(vector<string>{}, "Komanda tr mora da ima opciju"); }

    data = getInputString();
    string searchWord = cutString(options[0].substr(1));
    string insertWord = arguments[0];
    string text = "",word="";
    int i = 0,n=data.size(),m=searchWord.size();
    bool checking = true;

    
    for (int j = 0; j < n;){
        if (j+m<=n && data.substr(j,m)==searchWord) {
            text += insertWord;
            j += m;
        }
        else {
            text += data[j++];
        }
    }

    *out << text <<"\n";
    out->flush();
}

void PipeCommand::execute()
{
    for (auto command : commands) {
        command->execute();
    }
}
