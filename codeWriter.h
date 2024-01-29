#ifndef CODEWRITER_H
#define CODEWRITER_H

#include <fstream>
#include "parser.h"

class CodeWriter {
public:
    CodeWriter(std::ofstream& output);
    void writeArithmetic(const std::string& command);
    void writePushPop(CommandType commandType, const std::string& segment, const std::string& index);
    void close();
private:
    std::ofstream& output;
};

#endif
