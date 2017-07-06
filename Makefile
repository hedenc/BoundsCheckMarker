
SOFTBOUND_CC = clang
SOFTBOUND_FLAGS = -fsoftboundcets -lm -lrt -g
SOFTBOUND_LIB = 'home/hedenc/softboundcets-34-master/softboundcets-lib'

CXXFLAGS = -std=c++14 -O3 -Wall -pedantic

COREUTILS_DIR = ./Tests/coreutils-8.27/src

BINFILES = $(filter-out \
	$(wildcard $(COREUTILS_DIR)/*.*) \
	$(shell find $(COREUTILS_DIR)/ -maxdepth 1 -type d) \
	$(COREUTILS_DIR)/dcgen \
	$(COREUTILS_DIR)/extract-magic \
	, \
	$(wildcard ./Tests/coreutils-8.27/src/*))

tests: marker $(BINFILES) Tests/test-output.txt
	$(foreach FILE, $(BINFILES), ./marker $(FILE) > $(FILE)-output.txt; )

all: marker tests

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

Tests/test-output.txt: Tests/test marker
	./marker Tests/test > Tests/test-output.txt

clean:
	rm -f *.o marker

hard_clean: clean
	rm -f 19.def afAF.def gzGZ.def

