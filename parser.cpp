/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#include "parser.hpp"

#include <cstdio>
#include <stdexcept>

#define STR(X) #X

namespace rtboundsmark {

void parser::parse() 
{
    while (eat_until({call}))
        parse_call();
}

// Parses when matching [call]
bool parser::parse_call()
{
    if (!eat_until({at}) || !expects({func}) )
        return false;

    char *strnode = new char [strlen(lex_.strval()) +1];
    strcpy(strnode, lex_.strval());

    if (!expects({lparen}))
        throw std::runtime_error(
            "Expected ( after function name " __FILE__ ":" STR(__LINE__)
        );

    // Parse arguments, we need to handle depth of parantheses
    size_t depth = 0;
    size_t arg = 0;
    do {
        switch (ctok_) {
        case lparen:
            ++depth;
            break;
        case rparen:
            --depth;
            break;
        case comma:
            if (expects({i32, intlit})) {
                // 'arg' will in the end hold last matching argument
                arg = lex_.intval(); 
                break;
            }
            continue; // continue as to not eat the non matching token
        case eof:
            throw std::runtime_error(
                "Unbalanced parantheses in function" __FILE__ ":" STR(__LINE__)
            );
        default:
            ;
        }
        eat_until({lparen, rparen, comma});
    } while (depth);

    printf("Line %4zu %s\n", arg, strnode);

    delete [] strnode;
    return true;
}

/*
Takes a list of tokens that are expected in order by the parser
Returns true if match
*/
bool parser::expects(std::initializer_list<token> toks)
{
    for (auto it : toks) {
        ctok_ = lex_.next_tok();
        if (ctok_ != it)
            return false;
    }
    return true;
}

/*
Consumes tokens until any of the argument tokens are found
Returns false if EOF is found
*/
bool parser::eat_until(std::initializer_list<token> until)
{
    do {
        ctok_ = lex_.next_tok();
        if (ctok_ == eof)
            break;
        for (auto it : until) {
            if (ctok_ == it)
                return true;
        }
    } while (true);
    return false;
}


} /* namespace */


