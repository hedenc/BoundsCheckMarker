
SOFTBOUND_CC = clang
SOFTBOUND_FLAGS = -fsoftboundcets -lm -lrt -g
SOFTBOUND_LIB = 'home/hedenc/softboundcets-34-master/softboundcets-lib'

CXXFLAGS = -std=c++14 -O3 -Wall -pedantic

all: marker

marker: marker.o lexer.o parser.o
	$(CXX) $(CXXFLAGS) -o marker marker.o lexer.o parser.o

marker.o: marker.cpp parser.hpp
	$(CXX) -c $(CXXFLAGS) -o marker.o marker.cpp

lexer.o: lexer.cpp lexer.hpp afAF.def keywords.def 19.def gzGZ_.def
	$(CXX) -c $(CXXFLAGS) -o lexer.o lexer.cpp

gzGZ_.def: gzGZ_.py
	python3 gzGZ_.py > gzGZ_.def

afAF.def: afAF.py
	python3 afAF.py > afAF.def

19.def: 19.py
	python3 19.py > 19.def

parser.o: parser.cpp lexer.hpp functions.def
	$(CXX) -c $(CXXFLAGS) -o parser.o parser.cpp

Tests/test: Tests/test.c
	$(SOFTBOUND_CC) $(SOFTBOUND_FLAGS) -L/$(SOFTBOUND_LIB) -o ./Tests/test ./Tests/test.c

clean:
	rm -f *.o marker

hard_clean: clean
	rm -f 19.def afAF.def gzGZ.def

