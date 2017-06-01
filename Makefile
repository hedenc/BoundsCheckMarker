
CXXFLAGS = -std=c++14 -O3 -Wall -pedantic

all: main

main: main.o lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o main main.o lexer.o parser.o

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
	rm -f *.o main alpha.def num.def
