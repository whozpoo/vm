#ifndef PARSER_H
#define PARSER_H

#include <fstream>

enum class CommandType {
    C_ARITHMETIC,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL,
    TEST
};

class Parser {
public:
    Parser(std::ifstream& input);
    bool hasMoreCommands();
    void advance();
    CommandType commandType();
    std::string arg1();
    std::string arg2();
private:
    std::ifstream& input;
    std::vector<std::string> currentCommand;
};

#endif