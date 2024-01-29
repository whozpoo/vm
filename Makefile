CXX := clang++

TARGET := vm

SRC := VMTranslator.cpp parser.cpp codeWriter.cpp helpers.cpp

$(TARGET): $(SRC)
	$(CXX) -std=c++20 -Wall -Wextra -pedantic $(SRC) -o $(TARGET) 

clean:
	rm -f $(TARGET) test.asm