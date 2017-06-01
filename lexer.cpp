/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#include "lexer.hpp"

namespace rtboundsmark {

// Hashmap of tokens per keyword
const std::unordered_map<std::string, token> lexer::keywords_ = {
    {"dbg", dbg},
    {"metadata", meta},
    {"call", call},
    {"i32", i32},
    {"null", null}
// Map functions to track to token 'func'
#define X(name) ,{ #name , func }
#include "functions.def"
#undef X
};

lexer::lexer(const char *fname): head_set_(false), val_()
{
    input_ = fopen(fname, "r");
    if (!input_)
        fprintf(stderr, "Error opening file at %s:%d\n", __FILE__, __LINE__);
}

// Moves first non eaten character to 'head_'
void lexer::read()
{
    if (!head_set_){
        head_ = fgetc(input_);
        head_set_ = 1;
    }
}

token lexer::next_tok()
{
    do {
        read();

        switch (head_) {
        case EOF:
            return eof;
        case ' ': 
        case '\n':
        case '\r':
        case '\t':
            eat();
            break;

#define X(letter) case letter:
#include "alpha.def"
#undef X
        /* Generates
        case a...z || A...Z || _:
        */
            eat();
            val_ = head_;
            return state1();

#define X(num) case num:
#include "num.def"
#undef X
        /* Generates
        case 0-9: 
        */
            eat();
            val_ = head_;
            return state2();
/*
Generates blocks for listed tokens with following code:
            eat();
            return tok;
*/
#define X(sym, tok) case sym: eat(); return tok;
X('!', bang)
X('}', rbrace)
X(',', comma)
X('{', lbrace)
X('=', eqsign)
X('@', at)
X('(', lparen)
X(')', rparen)
#undef X
        default:
            eat();
            return other;
        }
    } while (true);
}

// Handles the state when [a-zA-Z_] is matched
token lexer::state1()
{

    do {
        read();
        switch (head_) {
#define X(letter) case letter:
#include "alpha.def"
#undef X
#define X(num) case num:
#include "num.def"
#undef X
        /* Generates
        case a...z || A...Z || _ || 0...9:
        */
            val_ += head_;
            eat();
            break;
        default:
            auto ev_kwd = keywords_.find(val_.c_str());
            return (ev_kwd != keywords_.end()) ? ev_kwd->second : other;
        }
    } while (true);
}

// Handles the state when [0-9] is matched
token lexer::state2()
{
    do {
        read();
        switch (head_) {
#define X(num) case num:
#include "num.def"
#undef X
        /* Generates
        case 0...9:
        */
            val_ += head_;
            eat();
            break;
        default:
            return intlit;
        }
    } while (true);
}

void lexer::print()
{
    token ctok;
    do {
        ctok = next_tok();
        switch (ctok){
        case intlit:
            printf("intlit[%llu]\n", intval());
            break;
        case dbg:
            printf("dbg\n");
            break;
        case func:
            printf("func[%s]\n", strval());
            break;
/*
Generates blocks for each token with following code
            printf("sym\n");
            break;
*/
#define X(sym) case sym: printf(#sym "\n"); break;
X(eof)
X(other)
X(bang)
X(rbrace)
X(comma)
X(meta)
X(lbrace)
X(eqsign)
X(at)
X(call)
X(i32)
X(null)
X(lparen)
X(rparen)
#undef X
        }
    } while (ctok != eof);
}

} /* namespace */
