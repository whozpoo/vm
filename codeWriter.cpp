#include <iostream>
#include "codeWriter.h"


using namespace std;

CodeWriter::CodeWriter(ofstream& output)
    : output{output}
{
}

void CodeWriter::writeArithmetic(const std::string& command)
{
    output << command << endl;
    return;
}

void CodeWriter::writePushPop(CommandType commandType, const std::string&, int index)
{
    return;
}

void CodeWriter::close()
{
    output.close();
    return;
}

