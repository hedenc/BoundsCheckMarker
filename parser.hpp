/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <initializer_list>

namespace rtboundsmark {

/*
Parser
parse() to parse
*/
class parser {
    lexer lex_;

    token ctok_; // Current token

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

    // Parses file and output marking info to stdout
    void parse();

};

} /* namespace */

#endif