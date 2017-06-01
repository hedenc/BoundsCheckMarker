
CXXFLAGS = -std=c++14 -O3 -Wall -pedantic
SOFTBOUND_CC = ../scinstall/bin/clang

all: marker examples

marker: main.o lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o marker main.o lexer.o parser.o

main.o: main.cpp parser.hpp
	$(CXX) -c $(CXXFLAGS) -o main.o main.cpp

lexer.o: lexer.cpp lexer.hpp functions.def alpha.def num.def
	$(CXX) -c $(CXXFLAGS) -o lexer.o lexer.cpp

parser.o: parser.cpp parser.hpp lexer.hpp functions.def
	$(CXX) -c $(CXXFLAGS) -o parser.o parser.cpp

alpha.def: alpha.py
	python3 alpha.py > alpha.def

num.def: num.py
	python3 num.py > num.def

clean:
	rm -f *.o main Examples/output.txt

examples: Examples/safe_test.s Examples/unsafe_test.s Examples/output.txt

Examples/safe_test.s: Examples/test.c
	$(SOFTBOUND_CC) -g -fmemsafety -S -emit-llvm -o Examples/safe_test.s Examples/test.c

Examples/unsafe_test.s: Examples/test.c
	$(SOFTBOUND_CC) -g -S -emit-llvm -o Examples/unsafe_test.s Examples/test.c

Examples/output.txt: marker Examples/safe_test.s
	./marker Examples/safe_test.s > Examples/output.txt