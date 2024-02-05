#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include "parser.h"

class CodeWriter {
public:
    CodeWriter(std::ofstream& output);
    void setFileName(const std::string& inputName);
    void writeArithmetic(const std::string& command);
    void writePushPop(CommandType commandType, const std::string& segment, const std::string& index);
    void writeLabel(const std::string& label);
    void writeGoto(const std::string& label);
    void writeIf(const std::string& label);
    void writeCall(const std::string& functionName, int numArgs);
    void writeFunction(const std::string& functionName, int numLocals);
    void writeReturn();
    void close();
private:
    std::ofstream& output;
    int counter;
    int currentCall;
    //std::string& filename;
};

#endif
