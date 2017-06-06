/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <queue>

#include <initializer_list>

namespace rtboundsmark {

/*
Parser
Construct empty AST and pass to parse() to parse
*/
class parser {
    lexer lex_;

    token ctok_; // Look ahead buffer

    bool parse_call();

    bool parse_bang();


    bool expects(std::initializer_list<token> toks);

    bool eat_until(std::initializer_list<token> until);


public:
    /*
    Constructs parser for file with name 'fname'
    */
    parser(const char *fname): 
        lex_(fname), ctok_(eof) 
    {};

    // Parses file and fills up the AST in 'tree'
    void parse();

};

} /* namespace */

#endif