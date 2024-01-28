CXX := clang++

TARGET := vm

SRC := VMTranslator.cpp parser.cpp codeWriter.cpp

$(TARGET): $(SRC)
	$(CXX) -std=c++20 -stdlib=libc++ -Wall -Wextra -pedantic $(SRC) -o $(TARGET) 

clean:
	rm -f $(TARGET)