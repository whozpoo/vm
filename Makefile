CXX := clang++

TARGET := vm

SRC := VMTranslator.cpp parser.cpp codeWriter.cpp helpers.cpp

RESULT := ./StackArithmetic/SimpleAdd/SimpleAdd.asm ./StackArithmetic/StackTest/StackTest.asm ./MemoryAccess/BasicTest/BasicTest.asm ./MemoryAccess/PointerTest/PointerTest.asm ./MemoryAccess/StaticTest/StaticTest.asm

$(TARGET): $(SRC)
	$(CXX) -std=c++20 -Wall -Wextra -pedantic $(SRC) -o $(TARGET) 

clean:
	rm -f $(TARGET) test.asm
	rm -f $(RESULT)
