#include <iostream>
#include <sstream>
#include <regex>
#include "parser.h"

using namespace std;

Parser::Parser(ifstream& input, string inputName)
    : input{input}
{
    regex pattern{R"(\..*$)"};
    filename = regex_replace(inputName, pattern, "");
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
            currentCommand.clear();
            return;
        }
    }
    currentLine.pop_back();

    currentCommand.clear();

    // split the line into tokens by space
    istringstream iss(currentLine);
    while (iss >> token) {
        if (token == "//") break;
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
    else if (currentCommand.size() == 2) {
        if (currentCommand[0] == "label") {
            return CommandType::C_LABEL;
        }
        else if (currentCommand[0] == "goto") {
            return CommandType::C_GOTO;
        }
        else if (currentCommand[0] == "if-goto") {
            return CommandType::C_IF;
        }
        else if (currentCommand[0] == "function") {
            return CommandType::C_FUNCTION;
        }
        else if (currentCommand[0] == "return") {
            return CommandType::C_RETURN;
        }
        else if (currentCommand[0] == "call") {
            return CommandType::C_CALL;
        }
        else {
            throw invalid_argument("Invalid command from commandType() size 2");
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
    else if (currentCommand.size() == 0) {
        return CommandType::EMPTY;
    }
    else {
        cout << "size: " << currentCommand.size() << endl;
        for (const auto& e : currentCommand) cout << e << endl;
        throw invalid_argument("Invalid size from commandType(). Command: " + currentCommand[0]);
    }
}

string Parser::arg1()
{
    // C_ARITHMETIC or COMMENT
    if (currentCommand.size() == 1) {
        return currentCommand[0];
    }
    else if (currentCommand.size() == 2) {
        if (currentCommand[0] == "label" || currentCommand[0] == "goto" || currentCommand[0] == "if-goto") {
            return currentCommand[0];
        }
    }
    else if (currentCommand.size() == 3) {
        string segment = currentCommand[1];
        if (segment == "local") {
            return "LCL";
        } else if (segment == "argument") {
            return "ARG";
        } else if (segment == "this") {
            return "THIS";
        } else if (segment == "that") {
            return "THAT";
        } else if (segment == "pointer") {
            return "pointer";
        } else if (segment == "temp") {
            return "temp";
        } else if (segment == "constant") {
            return "constant";
        } else if (segment == "static") {
            return "static";
        } else {
            throw invalid_argument("Invalid segment in arg1(): " + segment);
        }
    } else {
        throw invalid_argument("Invalid command from arg1()");
    }
}

string Parser::arg2()
{
    // push or pop
    if (currentCommand.size() == 3) {
        if (currentCommand[1] == "static") {
            return filename + "." + currentCommand[2];
        } else if (currentCommand[1] == "temp") {
            return to_string(5 + stoi(currentCommand[2]));
        } else if (currentCommand[1] == "pointer" && currentCommand[2] == "0") {
            return "THIS";
        } else if (currentCommand[1] == "pointer" && currentCommand[2] == "1") {
            return "THAT";
        }
        return currentCommand[2];
    }
    else throw invalid_argument("Invalid size from arg2()");
}