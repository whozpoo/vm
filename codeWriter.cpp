#include <iostream>
#include "codeWriter.h"
#include "helpers.h"


using namespace std;

CodeWriter::CodeWriter(ofstream& output)
    : output{output}, counter{0}, currentCall{0}, currentFunction{""}
{
    bootSP(output);
    output << "@Sys.init" << endl;
    output << "0;JMP" << endl;
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

// TODO: Multiple functions?
void CodeWriter::writeLabel(const string& label)
{
    output << "// LABEL" << endl;

    output << "(" << currentFunction << "$" << label << ")" << endl;
}

void CodeWriter::writeGoto(const string& label)
{
    output << "// GOTO" << endl;

    output << "@" << currentFunction << "$" << label << endl;
    output << "0;JMP" << endl;
}

void CodeWriter::writeIf(const string& label)
{
    output << "// IF-GOTO" << endl;

    popToD(output);
    output << "@" << currentFunction << "$" << label << endl;
    output << "D;JNE" << endl;
}

void CodeWriter::writeCall(const string& functionName, int numArgs)
{
    output << "// CALL" << endl;

    const string returnAddress = functionName + "$ret" + to_string(currentCall++);

    pushCallerFrame(output, returnAddress);

    setCalleePtrs(output, numArgs);
    
    // goto the callee
    output << "@" << functionName << endl;
    output << "0;JMP" << endl;

    // inject the return address label to the code
    output << "(" << returnAddress << ")" << endl;

}

void CodeWriter::writeFunction(const string& functionName, int numLocals)
{
    output << "// FUNCTION" << endl;

    output << "(" << functionName << ")" << endl;
    currentFunction = functionName;
    for (int i = 0; i < numLocals; ++i) pushToStack(output, "constant", "0");
}

/**
 * @brief Recover the environment of the caller function.
 * 
 * 
 */
void CodeWriter::writeReturn()
{
    output << "// RETURN" << endl;

    recoverCaller(output);

}

void CodeWriter::close()
{
    writeInfiniteLoop(output);
    output.close();
    return;
}
