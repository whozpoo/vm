#ifndef HELPERS_H
#define HELPERS_H

#include <fstream>


void pushToStack(std::ofstream& output, const std::string& segment, const std::string& index);
void popToSeg(std::ofstream& output, const std::string& segment, const std::string& index);

void writeSegValToD(std::ofstream& output, const std::string& segment, const std::string& index);
void writeDValToSeg(std::ofstream& output, const std::string& segment, const std::string& index);
void pushDToStack(std::ofstream& output);
void popToD(std::ofstream& output);

void incrementSP(std::ofstream& output);
void decrementSP(std::ofstream& output);

void writeArithmeticAdd(std::ofstream& output);
void writeArithmeticSub(std::ofstream& output);
void writeArithmeticNeg(std::ofstream& output);
void writeArithmeticEq(std::ofstream& output);
void writeArithmeticGt(std::ofstream& output);
void writeArithmeticLt(std::ofstream& output);
void writeArithmeticAnd(std::ofstream& output);
void writeArithmeticOr(std::ofstream& output);
void writeArithmeticNot(std::ofstream& output);

void pushBool(std::ofstream& output);

void writeInfiniteLoop(std::ofstream& output);
#endif