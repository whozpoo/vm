#include <iostream>
#include <sstream>
#include "parser.h"

using namespace std;

Parser::Parser(ifstream& input)
    : input{input}
{
}

bool Parser::hasMoreCommands()
{
    return !input.eof();
}

void Parser::advance()
{
    string currentLine, token;
    getline(input, currentLine);
    // the vm file uses \r\n for new line, so we need to remove the \r
    while (currentLine.empty() || currentLine[0] == '\r' || currentLine.substr(0, 2) == "//") {
        if (hasMoreCommands()) {
            getline(input, currentLine);
        }
        else {
            return;
        }
    }
    currentLine.pop_back();

    istringstream iss(currentLine);

    // split the line into tokens by space
    currentCommand.clear();
    while (getline(iss, token, ' ')) {
        currentCommand.push_back(token);
    }
}

CommandType Parser::commandType()
{
    if (currentCommand.size() == 1) {
        if (currentCommand[0] == "add" || currentCommand[0] == "sub" || currentCommand[0] == "neg" || currentCommand[0] == "eq" || currentCommand[0] == "gt" || currentCommand[0] == "lt" || currentCommand[0] == "and" || currentCommand[0] == "or" || currentCommand[0] == "not") {
            return CommandType::C_ARITHMETIC;
        }
        else {
            cout << "Invalid: " << currentCommand[0] << endl;
            throw invalid_argument("Invalid command from commandType() size 1");
        }
    }
    else if (currentCommand.size() == 3) {
        if (currentCommand[0] == "push") {
            return CommandType::C_PUSH;
        }
        else if (currentCommand[0] == "pop") {
            return CommandType::C_POP;
        }
        else {
            throw invalid_argument("Invalid command from commandType() size 3");
        }
    }
    else {
        throw invalid_argument("Invalid size from commandType()");
    }
}

string Parser::arg1()
{
    // C_ARITHMETIC
    if (currentCommand.size() == 1) {
        return currentCommand[0];
    }
    else if (currentCommand.size() == 3) {
        return currentCommand[1];
    }
    else {
        throw invalid_argument("Invalid command from arg1()");
    }
}

int Parser::arg2()
{
    return 0;
}