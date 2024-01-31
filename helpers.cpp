/**
 * Helper functions for codeWriter
 * 
 */
#include "helpers.h"

using namespace std;

int i = 0;

/**
 * @brief Push the value in {segment index} onto the stack.
 *  
 */
void pushToStack(ofstream& output, const string& segment, const std::string& index)
{
    output << "// PUSH" << endl;

    // put the value of seg i into D register
    writeSegValToD(output, segment, index);

    // put the value of D onto the stack
    pushDToStack(output);
}

void popToSeg(std::ofstream& output, const std::string& segment, const std::string& index)
{
    output << "// POP" << endl;

    // pop to D
    popToD(output);

    // save the value of D to seg i
    writeDValToSeg(output, segment, index);
}

/**
 * @brief Write the value of {segment index} into D register.
 * 
 *        A register points to the value of {segment index}.
 *        M register holds the value of {segment index}.
 */
void writeSegValToD(ofstream& output, const string& segment, const string& index)
{
    if (segment == "constant") {
        output << "@" << index << endl;
        output << "D=A" << endl;
    }
    else {
        output << "@" << segment << endl;
        output << "A=A+" << index << endl;
        output << "D=M" << endl;
    }
}

void writeDValToSeg(ofstream& output, const string& segment, const string& index)
{
    output << "@" << segment << endl;
    output << "A=A+" << index << endl;
    output << "M=D" << endl;
}

/**
 * @brief Push the value of D register onto the stack.
 * 
 *        A points to the top of the stack.
 *        M points to the value of the top.
 */
void pushDToStack(ofstream& output)
{
    output << "@SP" << endl;    // now A holds SP, and M holds the stack pointer.
    output << "A=M" << endl;    // now A points to the top of the stack, and M holds the value of the top.
    output << "M=D" << endl;        

    // Update stack pointer
    incrementSP(output);
}

/**
 * @brief Increment the stack pointer by 1.
 * 
 *        A points to the top of the stack.
 *        M points to the value of the top.
 */
void incrementSP(ofstream& output)
{
    output << "@SP" << endl;
    output << "M=M+1" << endl;   
}

void decrementSP(ofstream& output)
{
    output << "@SP" << endl;
    output << "M=M-1" << endl;
}

void writeArithmeticAnd(std::ofstream& output)
{
    output << "// AND" << endl;

    popToD(output);

    output << "A=A-1" << endl;
    output << "M=M&D" << endl;
}

void writeArithmeticOr(std::ofstream& output)
{
    output << "// OR" << endl;

    popToD(output);

    output << "A=A-1" << endl;
    output << "M=M|D" << endl;
}

void writeArithmeticNot(std::ofstream& output)
{
    output << "// NOT" << endl;

    output << "@SP" << endl;
    output << "A=M-1" << endl;
    output << "M=!M" << endl;    
}

/**
 * @brief Add up the uppermost 2 values in a stack
 * 
 *        A register points to the second value from the top of the stack.
 *        D register holds the value that was originally on top.
 *        M register holds the result of the addition.
 */
void writeArithmeticAdd(ofstream& output)
{
    output << "// ADD" << endl;

    popToD(output);
        
    // add D to the second top value of the stack
    output << "A=A-1" << endl;
    output << "M=M+D" << endl;
}

void writeArithmeticSub(ofstream& output)
{
    output << "// SUB" << endl;

    popToD(output);

    output << "A=A-1" << endl;
    output << "M=M-D" << endl;    
}

void writeArithmeticNeg(ofstream& output)
{
    output << "// NEG" << endl;

    output << "@SP" << endl;
    output << "A=M-1" << endl;
    output << "M=-M" << endl;
}

void writeArithmeticEq(ofstream& output, int& i)
{
    output << "// EQ" << endl;

    // if a - b != 0, jump to FALSE
    writeArithmeticSub(output);
    output << "D=M" << endl;
    output << "@FALSE" << i << endl;
    output << "D;JNE" << endl;
    pushBool(output, i);
}

void writeArithmeticGt(ofstream& output, int& i)
{
    output << "// GT" << endl;

    // if a - b <= 0, jump to FALSE
    writeArithmeticSub(output);
    output << "D=M" << endl;
    output << "@FALSE" << i << endl;
    output << "D;JLE" << endl;
    pushBool(output, i);
}

void writeArithmeticLt(ofstream& output, int& i)
{
    output << "// LT" << endl;

    // if a - b >= 0, jump to FALSE
    writeArithmeticSub(output);
    output << "D=M" << endl;
    output << "@FALSE" << i << endl;
    output << "D;JGE" << endl;
    pushBool(output, i);
}

/**
 * @brief Push true or false to the stack based on M register.
 *
 *        A points to the top of the stack.
 *        M points to the value of the top.
 *
 *        Duplicate labels? 
 */
void pushBool(ofstream& output, int& i)
{
    decrementSP(output);
    pushToStack(output, "constant", "-1");
    output << "@END" << i << endl;
    output << "0;JMP" << endl;

    // else, jump to pushing 0
    output << "(FALSE" << i << ")" << endl;
    decrementSP(output);
    pushToStack(output, "constant", "0");

    output << "(END" << i << ")" << endl;
    ++i;
}

/**
 * @brief Pop the top value of the stack into D register. 
 *
 *        SP is updated.
 *        A register points to the top value of the stack.
 *        M register also holds the top value of the stack.
 */
void popToD(ofstream& output)
{
    output << "@SP" << endl;        // Put SP into A, SP contains the stack pointer
    output << "AM=M-1" << endl;     // Update the stack pointer in SP, and save the stack pointer into A
    output << "D=M" << endl;        // 
}


void writeInfiniteLoop(ofstream& output)
{
    output << "(END)" << endl;
    output << "@END" << endl;
    output << "0;JMP" << endl;
}