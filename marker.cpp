#include "analyser.hpp"

using binmark::ast;

class pipeobj {
    FILE *pipe_;
public:
    pipeobj(const char *cmd, const char *mode): pipe_(popen(cmd, mode))
    {
        if (!pipe_) throw std::runtime_error (
            "Error trying to run \"objdump\" on given file"
        );
    }
    ~pipeobj()
    {
        pclose(pipe_);
    }

    FILE *operator()() const 
    {
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

    ast tree;

    analyse(asmdump(), tree);

    tree.print();


    return 0;
}
