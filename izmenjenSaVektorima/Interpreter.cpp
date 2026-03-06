#include "Interpreter.h"

void Interpreter::run() {
    Reader r;

    while (true) {
        try {
            Parser::getInstance()->printArgument();

            string line = r.getLine();

            if (r.isEof()) {
                break;
            }

            if (line.empty())continue;

            Command* d = Parser::getInstance()->parseCommand(line);
            if (d) {
                d->execute();
                delete d;
                cin.clear();
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
    }
}


