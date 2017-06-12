
CXXFLAGS = -std=c++14 -O3 -Wall -pedantic

# Change to path to the softbound installed clang
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

hard_clean: clean
	rm -f alpha.def num.def Examples/safe_test.s Examples/unsafe_test.s

examples: Examples/safe_test.s Examples/unsafe_test.s Examples/output.txt

Examples/safe_test.s: Examples/test.c
	$(SOFTBOUND_CC) -g -fmemsafety -S -emit-llvm -o Examples/safe_test.s Examples/test.c

Examples/unsafe_test.s: Examples/test.c
	$(SOFTBOUND_CC) -g -S -emit-llvm -o Examples/unsafe_test.s Examples/test.c

Examples/output.txt: marker Examples/safe_test.s
	./marker Examples/safe_test.s > Examples/output.txt

Examples/safe_test.o: Examples/test.c
	$(SOFTBOUND_CC) -g -fmemsafety -c -o Examples/unsafe_test.o Examples/test.c

Examples/safe_test: Examples/safe_test.o
	$(SOFTBOUND_CC)  -fmemsafety -o Examples/safe_test Examples/safe_test.o -L../scinstall/lib