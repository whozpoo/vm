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
    output << "// PUSH " << segment << " " << index << endl;

    // put the value of seg i into D register
    writeSegValToD(output, segment, index);

    // put the value of D onto the stack
    pushDToStack(output);
}

void popToSeg(std::ofstream& output, const std::string& segment, const std::string& index)
{
    output << "// POP " << segment << " " << index << endl;

    writeSegAddrToR13(output, segment, index);

    popToD(output);
    output << "@R13" << endl;
    output << "A=M" << endl;
    output << "M=D" << endl;
}

/**
 * @brief Write the value of {segment index} into D register.
 * 
 *        A register points to the value of {segment index}.
 *        M register holds the value of {segment index}.
 */
void writeSegValToD(ofstream& output, const string& segment, const string& index)
{
    // the value for a constant is the constant itself
    if (segment == "constant") {
        output << "@" << index << endl;
        output << "D=A" << endl;
    }
    // the value for a static is the address in its label
    // the value for a temp is the address in 5 + index
    // the value for a pointer is THIS or THAT
    else if (segment == "static" || segment == "temp" || segment == "pointer") {
        output << "@" << index << endl;
        output << "D=M" << endl;        
    }
    else {
        output << "@" << segment << endl;
        output << "D=M" << endl;
        output << "@" << index << endl;
        output << "A=A+D" << endl;
        output << "D=M" << endl;
    }
}

/**
 * @brief 
 * 
 *        A pointes to R13
 *        D points to the destination
 *        M points to the destination
 */
void writeSegAddrToR13(ofstream& output, const string& segment, const string& index)
{
    // the address for an static is it's label
    // the address for a temp is 5 + index
    // the address for a pointer is either THIS or THAT
    if (segment == "static" || segment == "temp" || segment == "pointer") {
        output << "@" << index << endl;
        output << "D=A" << endl;
    }
    // addresses for these segments are the baseAddress + index = RAM[segment] + index
    else {
        output << "@" << segment << endl;
        output << "D=M" << endl;
        output << "@" << index << endl;
        output << "D=A+D" << endl;      // now A contains the destination address
    }

    output << "@R13" << endl;
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

void pushReturnAddress(ofstream& output, const string& returnAddress)
{
    output << "@" << returnAddress << endl;
    output << "D=A" << endl;
    pushDToStack(output);
}

/**
 * @brief 
 *
 *        The status of the registers are the same as pushDToStack();
 * 
 * @param output 
 * @param functionName 
 * @param i 
 */
void pushCallerFrame(ofstream& output, const string& returnAddress)
{
    pushReturnAddress(output, returnAddress);

    // push to pointer to the local segment of the caller onto the stack
    output << "@LCL" << endl;
    output << "D=M" << endl;
    pushDToStack(output);

    output << "@ARG" << endl;
    output << "D=M" << endl;
    pushDToStack(output);

    output << "@THIS" << endl;
    output << "D=M" << endl;
    pushDToStack(output);

    output << "@THAT" << endl;
    output << "D=M" << endl;
    pushDToStack(output);    
}

/**
 * @brief Reset ARG and LCL for the callee.
 * 
 */
void setCalleePtrs(ofstream& output, int numArgs)
{
    output << "@SP" << endl;
    output << "D=M" << endl;
    output << "@" << numArgs << endl;
    output << "D=D-A" << endl;
    output << "@5" << endl;
    output << "D=D-A" << endl;

    output << "@ARG" << endl;
    output << "A=D" << endl;
}

void setCallerSP(ofstream& output)
{
    output << "@ARG" << endl;
    output << "D=M+1" << endl;
    output << "@SP" << endl;
    output << "M=D" << endl;
}

/**
 * @brief Update the seg variable for the caller and adjusts R13 register accordingly; it shifts 
 *        the R13 pointer to the previous position in the stack.
 *
 *        Assuming D holds the address of the caller's segment and 
 *        R13 contains a pointer to the caller's current frame segment.
 *
 *        Upon completion,
 *        A register and M register will contain the pointer to the caller's next frame segment.
 *        D register will be updated to the address of the caller's next segment.
 *        The order is THAT->THIS->ARG->LCL->returnAddress
 */
void setCallerSeg(ofstream& output, const string& seg)
{
    // repoint label like THAT
    output << "@" << seg << endl;
    output << "M=D" << endl;
    // maintain the pointer
    output << "@R13" << endl;
    output << "AM=M-1" << endl;
    output << "D=M" << endl;    
}


/**
 * @brief Go back to the returnAddress after THAT, THIS, LCL, ARG of the caller are recovered.
 * 
 */
void recoverCaller(ofstream& output)
{
    // save the pointer to caller's that segment in R13.
    output << "@LCL" << endl;
    output << "AD=M-1" << endl;
    output << "@R13" << endl;
    output << "AM=D" << endl;
    output << "D=M" << endl;

    setCallerSeg(output, "THAT");
    setCallerSeg(output, "THIS");
    setCallerSeg(output, "ARG");
    setCallerSeg(output, "LCL");

    // store returnAddress in A, and then go to returnAddress;
    output << "A=M" << endl;
    output << "0;JMP" << endl;
}


void writeInfiniteLoop(ofstream& output)
{
    output << "(END)" << endl;
    output << "@END" << endl;
    output << "0;JMP" << endl;
}