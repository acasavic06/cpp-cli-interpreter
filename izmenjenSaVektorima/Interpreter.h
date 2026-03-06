#include <iostream>
#include <fstream>

#include "Commands.h"
#include "Parser.h"
#include "Reader.h"
#include "Exceptions.h"

class Interpreter {
public:
    Interpreter()=default;
    ~Interpreter() {};

    void run();
    //void runBatch();
};