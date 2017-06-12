

# Change to path to the softbound install directory
SOFTBOUND_CC = ../../scinstall

CXXFLAGS = -std=c++14 -O3 -Wall -pedantic

all: marker Examples/test_output.txt

Examples/test.o: Examples/test.c
	$(SOFTBOUND_CC)/bin/clang -g -fmemsafety -c -o Examples/test.o Examples/test.c

Examples/test: Examples/test.o
	$(SOFTBOUND_CC)/bin/clang -flto -fmemsafety -o Examples/test Examples/test.o  -L$(SOFTBOUND_CC)/lib

Examples/test_output.txt: Examples/test marker
	./marker Examples/test > Examples/test_output.txt

marker: marker.o lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o marker marker.o lexer.o parser.o

marker.o: marker.cpp parser.hpp
	$(CXX) -c $(CXXFLAGS) -o marker.o marker.cpp

lexer.o: lexer.cpp lexer.hpp alpha.def num.def hexnum.def
	$(CXX) -c $(CXXFLAGS) -o lexer.o lexer.cpp

alpha.def: alpha.py
	python3 alpha.py > alpha.def

hexnum.def: hexnum.py
	python3 hexnum.py > hexnum.def

num.def: num.py
	python3 num.py > num.def

parser.o: parser.cpp lexer.hpp
	$(CXX) -c $(CXXFLAGS) -o parser.o parser.cpp

clean:
	rm -f *.o marker Examples/test.o Examples/test_output.txt

hard_clean: clean
	rm -f alpha.def num.def hexnum.def Examples/main.cpp

