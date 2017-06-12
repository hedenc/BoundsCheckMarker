#include "parser.hpp"

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

void parser::parse(func_node *nxtnode)
{
    int64_t r9d = -1, rsp = -1, r8 = -1, r9 = -1;
    while (eat_until({addr})) {
        uint64_t addr = lex_.hexval();
        if (expects({id})) {
            if (!strcmp(lex_.strval(), "callq") && expects({intlit, proc})) {
                // parse callq
                const char *funname = lex_.strval();
                if (!strcmp(funname, "_boundscheckui_debug")) {
                    *nxtnode = {func_node::boundscheckui_debug, r8, r9d};
                    return;
                } else if (!strcmp(funname, "_poolcheckui_debug")) {
                    *nxtnode = {func_node::poolcheckui_debug, r8, r9d};
                    return;
                } else if (!strcmp(funname, "_fastlscheck_debug")) {
                    *nxtnode = {func_node::fastlscheck_debug, r9, rsp};
                    return;
                }

            } else if (!strcmp(lex_.strval(), "movl") && expects({intconst})) {
                // parse movl
                int64_t movval = lex_.intval();
                if (expects({comma, reg}) && !strcmp(lex_.strval(), "r9d")) {
                    r9d = movval;
                } else if (
                    ctok_ == lparen && expects({reg}) && 
                    !strcmp(lex_.strval(), "rsp") && expects({rparen})
                ) {
                    rsp = movval;
                }
            } else if (!strcmp(lex_.strval(), "leaq") && expects({intlit})) {
                // parse leaq
                int64_t offset = lex_.intval();
                if (expects({lparen, reg}) && !strcmp(lex_.strval(), "rip") &&
                    expects({rparen, comma, reg})
                ) {
                    if (!strcmp(lex_.strval(), "r8"))
                        r8 = offset + (addr & 0xFFFF) + 7;
                    else if (!strcmp(lex_.strval(), "r9"))
                        r9 = offset + (addr & 0xFFFF) + 7;
                }
            }
        }
    };
     *nxtnode = {func_node::eof};
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
