#include <iostream>
#include <fstream>
#include <filesystem>
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
            case CommandType::C_LABEL:
                codeWriter.writeLabel(parser.arg1());
                break;
            case CommandType::C_GOTO:
                codeWriter.writeGoto(parser.arg1());
                break;
            case CommandType::C_IF:
                codeWriter.writeIf(parser.arg1());
                break;
            case CommandType::C_CALL:
                codeWriter.writeCall(parser.arg1(), stoi(parser.arg2()));
                break;
            case CommandType::C_FUNCTION:
                codeWriter.writeFunction(parser.arg1(), stoi(parser.arg2()));
                break;
            case CommandType::C_RETURN:
                codeWriter.writeReturn();
                break;
            case CommandType::EMPTY:
                break;
            default:
                throw invalid_argument("Invalid command type from handleAllCommands()");
        }
    }
}

/**
 * @brief Compile a single vm file into the asm file.
 * 
 */
void compileFile(const string& path, CodeWriter& codeWriter)
{
    // Open input and output files
    ifstream input{path};
    if (!input.is_open()) {
        cout << "Error: Could not open input file" << endl;
        throw invalid_argument("Could not open input file");
    
    }

    Parser parser{input, path};
    handleAllCommands(parser, codeWriter);
}

/**
 * @brief Construct a new asm file out of vm files.
 * 
 * @param path - A folder that contains vm files or a single vm file.
 * @param output - The asm file.
 */
void compileFiles(const string& path, CodeWriter& codeWriter)
{
    if (filesystem::is_directory(path)) {
        // for all vm files
        for (const auto& entry : filesystem::directory_iterator(path)) {
            if (entry.is_regular_file() && entry.path().extension() == ".vm") compileFile(entry.path(), codeWriter);
        }
    }
    else {
        compileFile(path, codeWriter);
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

    ofstream output{argv[2]};
    if (!output.is_open()) {
        cout << "Error: Could not open output file" << endl;
        return 1;
    }
    CodeWriter codeWriter{output};

    compileFiles(argv[1], codeWriter);

    codeWriter.close();
    return 0;
}
