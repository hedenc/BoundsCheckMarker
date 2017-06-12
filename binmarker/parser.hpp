#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

/*
Tracked function call node
*/
struct func_node {
    enum {
        eof, // If eof is reached
        boundscheckui_debug,
        poolcheckui_debug, 
        fastlscheck_debug
    } func_; // Which function
    int64_t offset_; // Address of string in binary storing the file string
    int64_t line_; // Line number
};

/*
Parser
parse() to parse
*/

class parser {
    lexer lex_;

    token ctok_;

    bool expects(std::initializer_list<token> toks);

    bool eat_until(std::initializer_list<token> until);

public:
    /*
    Constructs parser for opened file 'file'
    */
    parser(FILE *file):
        lex_(file), ctok_(eof)
    {};

    // Parses up till next call node found, sets 'nxtnode' to that
    void parse(func_node *nxtnode);



};

} /* namespace */

#endif // PARSER_HPP