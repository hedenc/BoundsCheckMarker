#include "lexer.hpp"

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

// Moves first non eaten character to 'head_'
void lexer::read()
{
    if (!head_set_){
        head_ = fgetc(input_);
        head_set_ = 1;
    }
}

token lexer::return_addr()
{
    if (!start_addr)
        start_addr = hexval();
    else if (hexval() == start_addr)
        return eof;
    return addr;
}

token lexer::next_tok()
{
    do {
        read();
        switch (head_) {
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
        case '-':
            eat();
            val_ = head_;
            return state8();
        case '%':
            eat();
            val_ = {};
            return state4();
        case '$':
            eat();
            val_ = {};
            return state7();
        case '<':
            eat();
            val_ = {};
            return state6();
/*
Generates blocks for listed tokens with following code:
            eat();
            return tok;
*/
#define X(sym, tok) case sym: eat(); return tok;
X(',', comma)
X('(', lparen)
X(')', rparen)
#undef X
        case EOF:
            return eof;
        default:
            eat();
            return other;
        }
    } while (true);
}

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
            return id;
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
#define X(hex) case hex:
#include "hexnum.def"
#undef X
        /* Generates
        case a...f
        */
            val_ += head_;
            eat();
            return state3();
        case (':'):
            eat();
            return return_addr();
        default:
            return intlit;
        }
    } while (true);
}

token lexer::state3()
{
    do {
        read();
        switch (head_) {
#define X(num) case num:
#include "num.def"
#undef X
#define X(hex) case hex:
#include "hexnum.def"
#undef X
        /* Generates
        case 0...f:
        */  
            val_ += head_;
            eat();
            break;
        case (':'):
            eat();
            return return_addr();
        default:
            return other;
        }
    } while (true);
}

token lexer::state4()
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
            return reg;
        }
    } while (true);
}

token lexer::state5()
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
            return intconst;
        }
    } while (true);
}

token lexer::state6()
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
        case '>':
            eat();
            return proc;
        default:
            return other;
        }
    } while (true);
}

token lexer::state7()
{
    do {
        read();
        switch (head_) {
#define X(num) case num:
#include "num.def"
#undef X
        case ('-'):
        /* Generates
        case 0...9:
        */
            val_ += head_;
            eat();
            return state5();
        default:
            return intconst;
        }
    } while (true);
}

// Handles the state when [0-9] is matched
token lexer::state8()
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
            printf("intlit[%lli]\n", intval());
            break;
        case addr:
            printf("addr[%llu]\n", hexval());
            break;
        case reg:
            printf("reg[%s]\n", strval());
            break;
        case intconst:
            printf("intconst[%lli]\n", intval());
            break;
        case proc:
            printf("proc[%s]\n", strval());
            break;
        case id:
            printf("id[%s]\n", strval());
            break;
/*
Generates blocks for each token with following code
            printf("sym\n");
            break;
*/
#define X(sym) case sym: printf(#sym "\n"); break;
X(eof)
X(other)
X(comma)
X(lparen)
X(rparen)
#undef X
        }
    } while (ctok != eof);
}

} /* namespace */


