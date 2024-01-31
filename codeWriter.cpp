#include <iostream>
#include "codeWriter.h"
#include "helpers.h"


using namespace std;

CodeWriter::CodeWriter(ofstream& output)
    : output{output}, counter{0}
{
}

void CodeWriter::writeArithmetic(const string& command)
{   
    if (command == "add") writeArithmeticAdd(output);
    else if (command == "sub") writeArithmeticSub(output);
    else if (command == "neg") writeArithmeticNeg(output);
    else if (command == "eq") writeArithmeticEq(output, counter);
    else if (command == "gt") writeArithmeticGt(output, counter);
    else if (command == "lt") writeArithmeticLt(output, counter);
    else if (command == "and") writeArithmeticAnd(output);
    else if (command == "or") writeArithmeticOr(output);
    else if (command == "not") writeArithmeticNot(output);
    else throw invalid_argument("Invalid command from writeArithmetic()");
    return;
}

void CodeWriter::writePushPop(CommandType commandType, const string& segment, const string& index)
{
    if (commandType == CommandType::C_PUSH) pushToStack(output, segment, index);
    else if (commandType == CommandType::C_POP) popToSeg(output, segment, index);
    else throw invalid_argument("Invalid command type from writePushPop()");
    return;
}

void CodeWriter::close()
{
    writeInfiniteLoop(output);
    output.close();
    return;
}
