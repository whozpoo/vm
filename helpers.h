#ifndef HELPERS_H
#define HELPERS_H

#include <fstream>


void pushToStack(std::ofstream& output, const std::string& segment, const std::string& index);
void popToSeg(std::ofstream& output, const std::string& segment, const std::string& index);

void writeSegValToD(std::ofstream& output, const std::string& segment, const std::string& index);
void writeSegAddrToR13(std::ofstream& output, const std::string& segment, const std::string& index);
void pushDToStack(std::ofstream& output);
void popToD(std::ofstream& output);

void incrementSP(std::ofstream& output);
void decrementSP(std::ofstream& output);

void writeArithmeticAdd(std::ofstream& output);
void writeArithmeticSub(std::ofstream& output);
void writeArithmeticNeg(std::ofstream& output);
void writeArithmeticEq(std::ofstream& output, int& i);
void writeArithmeticGt(std::ofstream& output, int& i);
void writeArithmeticLt(std::ofstream& output, int& i);
void writeArithmeticAnd(std::ofstream& output);
void writeArithmeticOr(std::ofstream& output);
void writeArithmeticNot(std::ofstream& output);

void pushBool(std::ofstream& output, int& i);

void writeInfiniteLoop(std::ofstream& output);
#endif