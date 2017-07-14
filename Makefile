
SOFTBOUND_CC = clang 
SOFTBOUND_FLAGS = -fsoftboundcets -lm -lrt -g -flto -O3 
SOFTBOUND_LIB = 'home/hedenc/softboundcets-34-master/softboundcets-lib/lto'

CXXFLAGS = -std=c++14 -O3 -Wall -pedantic -Wno-gnu-case-range

COREUTILS_DIR = ./Tests/coreutils-lto/src

BINFILES = $(filter-out \
	$(wildcard $(COREUTILS_DIR)/*.*) \
	$(COREUTILS_DIR)/blake2 \
	$(COREUTILS_DIR)/dcgen \
	$(COREUTILS_DIR)/extract-magic \
	, \
	$(wildcard $(COREUTILS_DIR)/*))

tests: marker Tests/test-output.txt
	$(foreach FILE, $(BINFILES), ./marker $(FILE) > $(FILE)-output.txt; )

all: marker tests

marker: marker.o lexer.o parser.o code_block.o analyser.o
	$(CXX) $(CXXFLAGS) -o marker marker.o lexer.o parser.o code_block.o analyser.o

marker.o: marker.cpp analyser.hpp
	$(CXX) -c $(CXXFLAGS) -o marker.o marker.cpp

analyser.o: analyser.cpp parser.hpp analyser.hpp
	$(CXX) -c $(CXXFLAGS) -o analyser.o analyser.cpp

lexer.o: lexer.cpp lexer.hpp keywords.def
	$(CXX) -c $(CXXFLAGS) -o lexer.o lexer.cpp

parser.o: parser.cpp lexer.hpp functions.def code_block.hpp
	$(CXX) -c $(CXXFLAGS) -o parser.o parser.cpp

code_block.o: code_block.cpp code_block.hpp parser.hpp
	$(CXX) -c $(CXXFLAGS) -o code_block.o code_block.cpp

Tests/test: Tests/test.c
	$(SOFTBOUND_CC) $(SOFTBOUND_FLAGS) -L/$(SOFTBOUND_LIB) -o ./Tests/test ./Tests/test.c

Tests/test-output.txt: Tests/test marker
	./marker Tests/test > Tests/test-output.txt

clean:
	rm -f *.o marker

hard_clean: clean
	rm -f

