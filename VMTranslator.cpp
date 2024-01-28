#include <iostream>
#include <fstream>
#include "parser.h"
#include "codeWriter.h"

using namespace std;

/**
 * @brief Parse all VM commands in the input file and write its corresponding Assembly command to the output file.
 * 
 * @param parser - It has an ifstream object that points to the input file. After parsing a line, it advances to the next line.
 * @param codeWriter - It has an ofstream object that points to the output file.
 */
void handleAllCommands(Parser& parser, CodeWriter& codeWriter) {
    while (parser.hasMoreCommands()) {
        // Parse the next line
        parser.advance();

        // Parse line
        CommandType commandType = parser.commandType();

        // Write to output file
        switch (commandType) {
            case CommandType::C_ARITHMETIC:
                codeWriter.writeArithmetic(parser.arg1());
                break;
            case CommandType::C_PUSH:
            case CommandType::C_POP:
                codeWriter.writePushPop(commandType, parser.arg1(), parser.arg2());
                break;
            default:
                throw invalid_argument("Invalid command type from handleAllCommands()");

        }
    }
}


/**
 * @brief Convert VM code to Assembly code.
 * 
 * @param argc - 3
 * @param argv - <file.vm> <file.asm>
 * @return int - 0 if successful, 1 if not
 */
int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: VMTranslator <file.vm> <file.asm>" << endl;
        return 1;
    }

    // Open input and output files
    ifstream input(argv[1]);
    ofstream output(argv[2]);
    if (!input.is_open()) {
        cout << "Error: Could not open input file" << endl;
        return 1;
    }
    if (!output.is_open()) {
        cout << "Error: Could not open output file" << endl;
        return 1;
    }

    // Read input file line by line
    Parser parser(input);
    CodeWriter codeWriter(output);
    handleAllCommands(parser, codeWriter);
    codeWriter.close();

    return 0;
}
