/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#include <cstdio>
#include "parser.hpp"

using rtboundsmark::ast;
using rtboundsmark::parser;
using rtboundsmark::call_node;
using rtboundsmark::dbg_node;

int main(int argc, const char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Provide filename of input file\n");
        return 1;
    }
    ast ast;

    parser(argv[1]).parse(ast);

    auto a = ast.begin();

    while (a != ast.end()) {
        if (a->kind_ == call_node) {
            a->print(); // Printing first call
            ++a;

            // Printing calls before first following line debug point
            while (a->kind_ == call_node) {
                printf(",\n");
                a->print();
                ++a;
            };

            // Printing line info for debug point
            printf("\n    invoked at ");
            if (a->kind_ == dbg_node) {
                auto search = ast.lineinfo().find(a->dbg_no());
                if (search != ast.lineinfo().end()) {
                    search->second.print();
                }
            }
            else // Failed to retrieve line info
                printf("?:?");
            printf("\n");
        }
        ++a;
    }
    return 0;
}
