#include "Parser.h"
#include "Exceptions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>

Parser* Parser::instance = nullptr;

Parser::Parser() : in(&cin), out(&cout), argument("$ "){
    mapa["time"] = &Parser::parseZeroComm;
    mapa["date"] = &Parser::parseZeroComm;
    mapa["echo"] = &Parser::parseOneComm;
    mapa["prompt"]= &Parser::parseOneComm;
    mapa["wc"] = &Parser::parseTwoComm;
    mapa["head"] = &Parser::parseTwoComm;
    mapa["tr"] = &Parser::parseThreeComm;
    mapa["touch"] = &Parser::parseFileComm;
    mapa["rm"] = &Parser::parseFileComm;
    mapa["truncate"] = &Parser::parseFileComm;
    mapa["batch"] = &Parser::parseFileComm;
}

Command* Parser::parseCommand(string line,istream* in,ostream* out, ostream* errorOut,bool isBatch) {
    inBatch = isBatch;
    if (indexPipe(line)!=-1) return parsePipeCommand(line,in,out);
    vector <string> tokens = tokenize(line);

    auto it = mapa.find(tokens[0]);
    if (it == mapa.end()) throw WrongCommandName(tokens);

    this->in = in;
    this->out = out;

    return (this->*(it->second))(tokens);
}

Command* Parser::parsePipeCommand(string line, istream* in, ostream* out, ostream* outError)
{
    vector<string> tokens=tokenizePipeCommand(line);
    /*      URADI PROVERE DA LI PRVI CLAN IMA SAMO INPUT, 
    POSLEDNJI SAMO OUTPUT, i da svi ovi izmedju nemaju nista, ni input ni output*/
    vector<Command*>niz;

    ostream* currOut;
    istream* currIn=in;
    
    checkPipeErrors(tokens);

    for (int i = 0; i < tokens.size(); i++) {
        if (i == 0) {
            stringstream* s = new stringstream();
            currOut = s;
            niz.push_back(parseCommand(tokens[i], currIn, currOut));
        }
        else if (i == tokens.size()-1) {
            currOut = out;
            niz.push_back(parseCommand(tokens[i], currIn, currOut));
        }
        else {
            stringstream* s = new stringstream();
            currOut = s;
            niz.push_back(parseCommand(tokens[i], currIn, currOut));
        }
        currIn = dynamic_cast<istream*>(currOut);
    }

    return new PipeCommand(in, out, niz);
}


vector<string> Parser::tokenize(string line) {
    vector <string> tokens;
    string token;
    bool quotesOn = false;
    set <char> zabranjeniKarakteri = { '$','%','&','@','+','*','?'};

    for (char c : line) {
        if (c == '"') {
            quotesOn = !quotesOn;
            token.push_back(c);
        }
        else if (!quotesOn && zabranjeniKarakteri.count(c)) {
            throw InvalidCharError(vector<string>{},line);
        }
        else if (isspace((c)) && !quotesOn) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else {
            token.push_back(c);
        }
    }
    if (!token.empty()) tokens.push_back(token);

    if (quotesOn) throw QuotesOnError(vector<string>{});

    vector<string> tokeni;
    string currWord = "";
    bool isMerge = false;

    for (string word: tokens) {
        if (word == ">" || word == ">>" || word == "<") {
            currWord = word;
            isMerge = true;
        }
        else {
            if (isMerge) {
                tokeni.push_back(currWord + word);
                isMerge = false;
            }
            else {
                tokeni.push_back(word);
            }
        }
    }

    return tokeni;
}

vector<string> Parser::tokenizePipeCommand(string line)
{
    vector<vector<string>> tokens;
    vector<string> commands;
    
    string text = line;
    int i = indexPipe(text); 

    while (i!=-1){
        commands.push_back(removeEmptySpace(text.substr(0,i)));
        text = removeEmptySpace(text.substr(i+1));
        i = indexPipe(text);
    } 
    commands.push_back(removeEmptySpace(text));

    checkEmptyPipeCmd(commands);

    return commands;
/*
    for (string command : commands) {
        tokens.push_back(tokenize(command));
    }

    return tokens;*/
}


Command* Parser::parseZeroComm(vector<string> tokens) {
    vector<string> pom(tokens.begin() + 1, tokens.end());
    createStreams(pom);

    if (tokens[0] == "time")
        return new TimeCommand(in, out);

    if (tokens[0] == "date")
        return new DateCommand(in, out);

    return nullptr;
}

Command* Parser::parseOneComm(vector<string> tokens) {

    //in = createInputStream(tokens, tokens[1]);
    vector<string> pom(tokens.begin() + 1, tokens.end());
    createStreams(pom);
    //if (!in) return nullptr;    

    if (tokens[0] == "echo")
        return new EchoCommand(in, out);
    if (tokens[0] == "prompt")
        argument =cutString(tokens[1]);
    return nullptr;
}


Command* Parser::parseFileComm(vector<string> tokens) {
    
    if (tokens[0] == "touch") {
        return new TouchCommand(in, out, tokens[1]);
    }

    if (tokens[0] == "rm") {
        return new RmCommand(in, out, tokens[1]);
    }

    if (tokens[0] == "truncate") {
        return new TruncateCommand(in, out, tokens[1]);
    }

    if (tokens[0] == "batch") {
        if (tokens.size() == 3)createOutputStream(tokens[2]);
        return new BatchCommand(in, out, tokens[1], { tokens[1] });
    }

    return nullptr;
}

Command* Parser::parseTwoComm(vector<string> tokens) {

    //vector<string> options = { tokens[1] };
    vector<string> options = dodela(tokens);

    /*if (options.size() == 0) {
        options.push_back("-w");
    }*/

    if (tokens.size()>1 && tokens[tokens.size()-1][0] != '-') {
        //in = createInputStream(tokens, tokens[tokens.size()-1]);
        vector<string> pom(tokens.begin() + 1, tokens.end());
        createStreams(pom);
        //if (!in) return nullptr;
    }
    
    if (tokens[0] == "wc")
        return new WCCommand(in, out, options);

    /*if (tokens[0] == "tr")
        return new TrCommand(in, out, options);*/

    if (tokens[0] == "head")
        return new HeadCommand(in, out, options);


    return nullptr;
}

Command* Parser::parseThreeComm(vector<string> tokens)
{
    vector<string> options = dodela(tokens);

    vector<string> arguments=createArguments(tokens);

    vector<string> pom(tokens.begin() + 1, tokens.end());
    createStreams(pom);

    if (options.empty()) { throw PipeCmd(vector<string>{}, "Komanda tr mora da ima opciju\n"); }

    if (tokens[0] == "tr")
        if (options[0].size() == 1) {
            *out << "Ne moze da stavis samo - kao opciju u tr komandi\n";
            return nullptr;
        }
        return new TrCommand(in, out, options,arguments);

    return nullptr;
}

void Parser::createStreams(vector <string> tokens) {
    for (string token : tokens) {
        if (token[0] == '-')continue;
        if (token[0] == '<') {
            //string input = removeEmptySpace(token.substr(1));
            string input = token.substr(1);
            checkInputError(input);
        }
        else if (isString(token)) {
            if (in != &cin) throw DoubleInput({}, "");
            istringstream* ss = new istringstream(cutString(token));
            in=ss;
        }
        //MozdA MOZE i  else if (token[0] == '>')createOutputStream(token);
        else if (token[0] == '>' || token.substr(0, 2) == ">>")createOutputStream(token);
        else {
            checkInputError(token);
        }
    }
}

/*
istream* Parser::createInputStream(vector <string> tokens, string input) {
    if (isString(input)) {
        istringstream* ss = new istringstream(cutString(input));
        return ss;
    }
    ifstream* file;

    if (input[0] == '<') {
        string in = removeEmptySpace(input.substr(1, input.size() - 1));
        file = new ifstream(in);
    }
    else {
        file = new ifstream(input);
    }
    
    if (!file->is_open()) {
        delete file;
        throw FileNotFound(tokens, input);
    }

    return file;
}
*/
void Parser::createOutputStream(string output) {
    ofstream* file;
    if (output[1] == '>') {
        string out = output.substr(2);
        //string out = removeEmptySpace(output.substr(2));
        file = new ofstream(out, ios::out | ios::app);
        //file = new ofstream(out);
    }
    else if(output[0]=='>') {
        //string out = removeEmptySpace(output.substr(1));
        string out = output.substr(1);
        file = new ofstream(out);
    }
    else {
        throw WrongOutput({},output);
    }

    if (!file->is_open()) {
        delete file;
        throw FileNotFound({}, output);
    }

    if (out != &cout && !inBatch) {
        delete file;
        throw DoubleOutput({}, output);
    }

    out = file;
}


void Parser::checkInputError(string const input)
{
    if (in != &cin) {
        throw DoubleInput({}, input);
    }

    ifstream* file = new ifstream(input);

    if (!file->is_open()) {
        delete file;
        throw FileNotFound({}, input);
    }
    in = file;
}

void Parser::checkPipeErrors(vector <string>tokens) {
    set <string> zabranjenjeKomande = { "batch", "touch","rm","truncate","prompt"};

    for (int i = 0; i < tokens.size(); i++) {
        string line = removeEmptySpace(tokens[i]);
        int index = line.find(' ');
        
        string cmd = line;
        if (index != string::npos) cmd = line.substr(0, index);

        if (zabranjenjeKomande.count(cmd)) {
            throw PipeCmd(vector<string>{}, "U cevovodu ne sme da se pojavi komanda " + cmd + "\n");
        }

        if (i != 0 && cmd=="time") {
            throw PipeCmd(vector<string>{}, "U cevovodu ne sme da se pojavi komanda time, osim na prvom mestu\n");
        }

        if (i != 0 && cmd == "date") {
            throw PipeCmd(vector<string>{}, "U cevovodu ne sme da se pojavi komanda date, osim na prvom mestu\n");
        }
    }
}

void Parser::checkEmptyPipeCmd(vector<string> commands)
{
    for (string a : commands) {
        if (a.empty()) {
            throw PipeCmd({},"U cevovodu je uneta prazna komanda\n");
        }
    }
}

vector<string> Parser::createArguments(vector<string> &tokens)
{
    bool empty = true;
    vector<string> arguments;
    
    for (int i = 2; i < tokens.size();i++) {
        if (tokens[i][0] == '"') {
            arguments.push_back(cutString(tokens[i]));
            empty = false;
            tokens.erase(tokens.begin() + i);
        }
    }

    if (empty)arguments.push_back("");

    return arguments;
}

vector<string> Parser::dodela(vector<string> tokens) {
    vector<string> niz;
    for (string token : tokens) {
        if (token[0] == '-') {
            niz.push_back(token);
        }
    }
    return niz;
}
