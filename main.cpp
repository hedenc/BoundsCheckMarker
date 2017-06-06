/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#include <cstdio>
#include "parser.hpp"

using rtboundsmark::parser;

int main(int argc, const char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Provide filename of input file\n");
        return 1;
    }

    parser(argv[1]).parse();

    return 0;
}
