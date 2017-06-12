#include <cstdio>
#include "parser.hpp"

#include "unistd.h"

#include <stdexcept>
#include <string>
#include <cstring>

#include <unordered_map>
#include <cstdlib>

using binmark::parser;
using binmark::func_node;

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

int main(int argc, char **argv)
{
    if (argc < 2)
        throw std::runtime_error("Provide filename of binary file\n");

    if (argv[1][0] == '-') // Sanity check input
        throw std::runtime_error(
            "Expected a filename of binary as first argument\n"
        );
    


    // Opening piped process to get strings in binary
    std::string cmdbuff = std::string("strings -t d ") += argv[1]; // Command buffer

    FILE *strfile = popen(cmdbuff.c_str(), "r");
    if (!strfile)
        throw std::runtime_error(
            "Error trying to run \"strings\" on given file"
        );

    std::string buff; // Buffer holding the strings in binary

    // Reading to buffer
    int c;
    while ((c = fgetc(strfile)) != EOF)
        buff += c;

    char *addr = &buff[0];
    char *str;

    // Map of strings in binary by address
    std::unordered_map<size_t, char *> strings;

    /*
    Inserts strings in map
    CAUTION: This breaks the internal buffer of std::string buff by replacing 
    '\n' with '\0'.
    DO NOT UNDER ANY CIRCUMSTANCES USE buff as a std::string after this point!!!
    */
    do {
        str = strchr(addr, ' ');
        if (!str)
            break;
        ++str;
        char *nxtaddr = strchr(str, '\n');
        if (!nxtaddr)
            break;
        *nxtaddr++ = '\0';
        char *end;
        strings.insert({strtoull(addr, &end, 10), str});
        //printf("%llu %s\n", strtoull(addr, &end, 10), str);
        addr = nxtaddr;
    } while (true);

    // Closing pipe
    pclose(strfile);

    // Opening piped process objdump to get the assembly
    cmdbuff.assign("objdump -d --no-show-raw-insn ") += (argv[1]);
    FILE *asmdump = popen(cmdbuff.c_str(), "r");
    if (!asmdump)
        throw std::runtime_error(
            "Error trying to run \"objdump\" on given file"
        );

    func_node node; // Node to hold next call node
    parser asmparse(asmdump); // Parser
    do {
        asmparse.parse(&node);

        // String lookup
        auto search = strings.find(node.offset_);
        char *context = (search == strings.end()) ? (char *) "??" : search->second;

        switch (node.func_) {
        case func_node::eof:
            break; // End loop
        case func_node::boundscheckui_debug:
            printf("boundscheckui_debug at %s:%lli\n", context, node.line_);
            continue;
        case func_node::poolcheckui_debug:
            printf("poolcheckui_debug   at %s:%lli\n", context, node.line_);
            continue;
        case func_node::fastlscheck_debug:
            printf("fastlscheck_debug   at %s:%lli\n", context, node.line_);
            continue;
        }
        break;
    } while (true);

    // Close pipe
    pclose(asmdump);


    return 0;
}
