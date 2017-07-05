#include <cstdio>
#include "parser.hpp"

#include "unistd.h"

#include <stdexcept>
#include <string>
#include <cstring>

#include <unordered_map>
#include <cstdlib>
#include <map>
#include <cinttypes>

using binmark::parser;
using binmark::fline;

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

class pipeobj {
    FILE *pipe_;
public:
    pipeobj(const char *cmd, const char *mode): pipe_(popen(cmd, mode)){
        if (!pipe_) throw std::runtime_error (
            "Error trying to run \"objdump\" on given file"
        );
    }
    ~pipeobj() {
        pclose(pipe_);
    }

    FILE *operator()() const {
        return pipe_;
    }
};

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::runtime_error("Provide filename of binary file");

    if (argv[1][0] == '-') // Sanity check input
        throw std::runtime_error(
            "Expected a filename of binary as first argument"
        );
    
    std::string cmdbuff("objdump -D -l --no-show-raw-insn -j .text ");
    cmdbuff += argv[1];

    // Opening piped process objdump to get the assembly
    pipeobj asmdump(cmdbuff.c_str(), "r");

    std::multimap<fline, std::string> ast;

    parser(asmdump()).parse(ast);

    const char empty[] = "";
    const char *fname = empty;
    uint64_t lineno = 0;
    for (auto &it: ast) {
        if (strcmp(fname, it.first.fname_.c_str())) {
            fname = it.first.fname_.c_str();
            lineno = 0;
            printf("File %s:\n", fname);
        }
        if (lineno != it.first.lineno_) {
            lineno = it.first.lineno_;
            printf("Line %" PRIu64 ":\n", lineno);
        }
        printf("    %-32s%" PRIx64 "\n", it.second.c_str(), it.first.addr_);

    }


    return 0;
}
