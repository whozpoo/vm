vm:
	clang++ -std=c++20 -stdlib=libc++ -Wall -Wextra -pedantic -o vm VMTranslator.cpp parser.cpp codeWriter.cpp
